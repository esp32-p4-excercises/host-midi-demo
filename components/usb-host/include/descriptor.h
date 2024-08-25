#pragma once


namespace usb
{
    template<typename T>
    struct Descriptor
    {
    protected:
        const T* descriptor;
    public:
        T* get()
        {
            return descriptor;
        }
    };

    namespace descriptor
    {
        class Device : public Descriptor<usb_device_desc_t>
        {
        public:
            Device() = default;
            ~Device() = default;
        };
        class Config : Descriptor<usb_config_desc_t>
        {
        private:
            // usb_conf
        public:
            Config() = delete;
            Config(const usb_config_desc_t* desc){
                descriptor = desc;
            };
            ~Config() = default;
        };
        class String : Descriptor<usb_str_desc_t>
        {
            String() = default;
            ~String() = default;
        };
    } // namespace descriptor
    
} // namespace usb
