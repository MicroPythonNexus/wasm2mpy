/**
 * Author:    Volodymyr Shymanskyy
 * Created:   24.08.2024
 **/

#include "py/dynruntime.h"
#include "wasm.h"

// Instance of the WASM module
w2c_wasm module;

// Exported Functions

mp_obj_t mp_mod_time;
mp_obj_t mp_ticks_ms;
mp_obj_t mp_sleep_ms;

static mp_obj_t setup(void) {
    w2c_wasm_setup(&module);
    return mp_const_none;
}

static MP_DEFINE_CONST_FUN_OBJ_0(setup_obj, setup);

static mp_obj_t loop(void) {
    w2c_wasm_loop(&module);
    return mp_const_none;
}

static MP_DEFINE_CONST_FUN_OBJ_0(loop_obj, loop);

// Imported Functions

void w2c_wiring_stopWdt(struct w2c_wiring* wiring) {
}

void w2c_wiring_delay(struct w2c_wiring* wiring, u32 t) {
    mp_obj_t args[1];
    args[0] = mp_obj_new_int_from_uint(t);
    mp_call_function_n_kw(mp_sleep_ms, 1, 0, args);
}

u32 w2c_wiring_millis(struct w2c_wiring* wiring) {
    return mp_obj_get_int(mp_call_function_n_kw(mp_ticks_ms, 0, 0, NULL));
}

void w2c_wiring_pinMode(struct w2c_wiring* wiring, u32 pin, u32 mode) {
    // TODO
}

void w2c_wiring_digitalWrite(struct w2c_wiring* wiring, u32 pin, u32 value) {
    // TODO
}

void w2c_wiring_print(struct w2c_wiring* wiring, u32 offset, u32 len) {
    mp_printf(&mp_plat_print, "%.*s", len, (const uint8_t*)module.w2c_memory.data + offset);
}

// Module Initialization

__attribute__((weak))
void w2c_app_0x5Finitialize(w2c_wasm* module) {}

__attribute__((weak))
void w2c_app_0x5Fstart(w2c_wasm* module) {}

void os_print_last_error(const char* msg) {
    mp_printf(&mp_plat_print, "Error: %s\n", msg);
    abort();
}

void wasm_rt_trap_handler(wasm_rt_trap_t code) {
    mp_printf(&mp_plat_print, "Trap: %d\n", code);
    abort();
}

mp_obj_t mpy_init(mp_obj_fun_bc_t *self, size_t n_args, size_t n_kw, mp_obj_t *args) {
    // This must be first, it sets up the globals dict and other things
    MP_DYNRUNTIME_INIT_ENTRY

    mp_mod_time = mp_import_name(MP_QSTR_time, mp_const_none, 0);
    mp_ticks_ms = mp_load_attr(mp_mod_time, MP_QSTR_ticks_ms);
    mp_sleep_ms = mp_load_attr(mp_mod_time, MP_QSTR_sleep_ms);

    // Initialize the Wasm runtime
    wasm_rt_init();

    // Construct the module instance
    wasm2c_wasm_instantiate(&module, NULL);

    w2c_app_0x5Finitialize(&module);
    w2c_app_0x5Fstart(&module);

    // Make the function available in the module's namespace
    mp_store_global(MP_QSTR_setup, MP_OBJ_FROM_PTR(&setup_obj));
    mp_store_global(MP_QSTR_loop, MP_OBJ_FROM_PTR(&loop_obj));

    // This must be last, it restores the globals dict
    MP_DYNRUNTIME_INIT_EXIT
}
