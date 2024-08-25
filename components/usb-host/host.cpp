
#include "esp_log.h"

#include "host.h"

#define TAG ""

namespace usb
{
    esp_event_loop_handle_t event_loop;
    ESP_EVENT_DEFINE_BASE(USB_HOST_BASE_EVENT);

    namespace internal
    {
        IRAM_ATTR static void client_async_seq_task(void *param)
        {
            // Host *host = (Host *)param;
            printf("create async task\n");
            while (1)
            {
                uint32_t event_flags;
                if (ESP_OK == usb_host_lib_handle_events(portMAX_DELAY, &event_flags))
                {
                    if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS)
                    {
                        ESP_LOGI("", "No more clients\n");
                        do
                        {
                            if (usb_host_device_free_all() != ESP_ERR_NOT_FINISHED)
                                break;
                        } while (0);
                        // usb_host_uninstall();
                    }

                    if (event_flags & USB_HOST_LIB_EVENT_FLAGS_ALL_FREE)
                    {
                        ESP_LOGI("", "USB_HOST_LIB_EVENT_FLAGS_ALL_FREE\n");
                    }
                }
            }
            printf("delete task\n");
            vTaskDelete(NULL);
        }

#if CONFIG_USB_HOST_ENABLE_ENUM_FILTER_CALLBACK
        static bool filter(const usb_device_desc_t *dev_desc, uint8_t *bConfigurationValue)
        {
            return true;
        }
#endif
    } // namespace private

    esp_err_t Host::init(usb_host_client_event_cb_t cb, usb_host_enum_filter_cb_t fn)
    {
        esp_event_loop_args_t event_loop_args = {};
        event_loop_args.queue_size = 10;
        event_loop_args.task_name = "name";
        event_loop_args.task_priority = 1;
        event_loop_args.task_stack_size = 6 * 1024;
        event_loop_args.task_core_id = 0;
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_create(&event_loop_args, &event_loop));

        usb_host_config_t config = {
            .skip_phy_setup = false,
            .intr_flags = ESP_INTR_FLAG_LEVEL1,
#if CONFIG_USB_HOST_ENABLE_ENUM_FILTER_CALLBACK
            .enum_filter_cb = internal::filter
#else
            .enum_filter_cb = NULL
#endif
        };

#if CONFIG_USB_HOST_ENABLE_ENUM_FILTER_CALLBACK
        if (fn)
            config.enum_filter_cb = fn;
#endif
        esp_err_t err = usb_host_install(&config);
        ESP_LOGI(TAG, "install status: %s", esp_err_to_name(err));

        _client_event_cb = cb;
        if (err == ESP_OK)
            xTaskCreate(internal::client_async_seq_task, "async", 5 * 512, this, 20, NULL); // create high priority task to handle host lib events

        // open();

        return err;
    }

    void Host::open()
    {
        // const usb_host_client_config_t client_config = {
        //     .max_num_event_msg = 5,
        //     .async = {
        //         .client_event_callback = this->_client_event_callback,
        //         .callback_arg = this,
        //     }};

        // auto err = usb_host_client_register(&client_config, &client_hdl);
        // ESP_LOGI("", "client register status: %s", esp_err_to_name(err));
    }

    void Host::close()
    {
        // auto err = usb_host_client_deregister(client_hdl);
        // printf("close: %s\n", esp_err_to_name(err));
        // if (!err)
        //     client_hdl = NULL;
    }

} // namespace usb
