#include "exgpio.h"

#if INCLUDE_EXLIB_GPIO

void exgpio_init(uint64_t pin_mask, gpio_mode_t mode, gpio_pullup_t pullup,
                 gpio_pulldown_t pulldown, gpio_int_type_t int_type) {
    gpio_config_t cfg = {
        .intr_type = int_type,
        .mode = mode,
        .pin_bit_mask = pin_mask,
        .pull_down_en = pulldown,
        .pull_up_en = pullup,
    };
    ESP_ERROR_CHECK(gpio_config(&cfg));
}

void exgpio_deinit(gpio_num_t pin) { ESP_ERROR_CHECK(gpio_reset_pin(pin)); }

void exgpio_output_init(gpio_num_t pin) {
    exgpio_init(EXGPIO_PIN_MASK(pin), GPIO_MODE_OUTPUT, 0, 0, 0);
}

void exgpio_input_init(gpio_num_t pin) {
    exgpio_init(EXGPIO_PIN_MASK(pin), GPIO_MODE_INPUT, 1, 0, 0);
}

void exgpio_toggle(gpio_num_t pin) {
    ESP_ERROR_CHECK(gpio_set_level(pin, !gpio_get_level(pin)));
}

void exgpio_install_isr_service(void) {
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
}

void exgpio_isr_handler_add(gpio_num_t pin, gpio_isr_t isr_handler,
                            void *user_data) {
    ESP_ERROR_CHECK(gpio_isr_handler_add(pin, isr_handler, user_data));
}

/**
 * @note before calling this function, we should initialize the pin mode and
 * pull-up/pull-down settings.
 */
dedic_gpio_bundle_handle_t exgpio_dedic_gpio_bundle_init(int *gpio_array,
                                                         size_t array_size,
                                                         unsigned int out_en,
                                                         unsigned int in_en) {
    assert(gpio_array);
    assert(array_size);
    dedic_gpio_bundle_handle_t bundle_handle = NULL;
    // One has to enable at least input or output mode in "config" parameter.
    if (!out_en && !in_en)
        out_en = 1;
    dedic_gpio_bundle_config_t bundle_config = {
        .gpio_array = gpio_array,
        .array_size = array_size,
        .flags = {.out_en = out_en, .in_en = in_en}};
    ESP_ERROR_CHECK(dedic_gpio_new_bundle(&bundle_config, &bundle_handle));
    return bundle_handle;
}

void exgpio_dedic_bundle_deinit(dedic_gpio_bundle_handle_t bundle_handle) {
    ESP_ERROR_CHECK(dedic_gpio_del_bundle(bundle_handle));
}

/**
 * @param offset according to the order of the gpio_array, the offset of the
 * pins starts from zero.
 */
void exgpio_dedic_gpio_set_level(dedic_gpio_bundle_handle_t bundle_handle,
                                 uint32_t offset, int value) {
    assert(bundle_handle);
    dedic_gpio_bundle_write(bundle_handle, 1 << offset, value << offset);
}

int exgpio_dedic_gpio_get_level(dedic_gpio_bundle_handle_t bundle_handle,
                                uint32_t offset) {
    assert(bundle_handle);
    uint32_t first_offset = 0;
    ESP_ERROR_CHECK(dedic_gpio_get_in_offset(bundle_handle, &first_offset));
    return dedic_gpio_bundle_read_in(bundle_handle) >> (first_offset + offset);
}

#endif
