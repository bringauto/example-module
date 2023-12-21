#include <device_management.h>
#include <general_error_codes.h>

#include <example_module.hpp>

int get_module_number() {return MODULE_NUMBER;}

int is_device_type_supported(unsigned int device_type) {
	if(device_type == BUTTON_DEVICE_TYPE) {
		return OK;
	}
	return NOT_OK;
}