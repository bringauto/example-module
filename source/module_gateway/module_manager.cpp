#include <example_module.hpp>

#include <fleet_protocol/common_headers/device_management.h>
#include <fleet_protocol/module_maintainer/module_gateway/module_manager.h>
#include <fleet_protocol/common_headers/memory_management.h>

#include <string>
#include <cstring>


void pressed_true(struct buffer *buffer) {
	const char *command = "{\"lit_up\": true}";
	size_t size = strlen(command) ;
	allocate(buffer, size);
	std::memcpy(buffer->data, command, buffer->size_in_bytes);
}

void pressed_false(struct buffer *buffer) {
	const char *command = "{\"lit_up\": false}";
	size_t size = strlen(command) ;
	allocate(buffer, size);
	std::memcpy(buffer->data, command, buffer->size_in_bytes);
}

int
send_status_condition(const struct buffer current_status, const struct buffer new_status, unsigned int device_type) {
	auto curr_data = static_cast<char *>(current_status.data);
	auto new_data = static_cast<char *>(new_status.data);
	if(strncmp(curr_data, new_data, std::max(current_status.size_in_bytes, new_status.size_in_bytes)) == 0) {
		return NOT_OK;
	}
	return OK;
}

int generate_command(struct buffer *generated_command, const struct buffer new_status,
					 const struct buffer current_status, const struct buffer current_command,
					 unsigned int device_type) {
	const char *data = static_cast<char *>(new_status.data);
	if(strncmp(data, "{\"pressed\": false}", new_status.size_in_bytes) == 0) {
		pressed_true(generated_command);
	} else if(strncmp(data, "{\"pressed\": true}", new_status.size_in_bytes) == 0) {
		pressed_false(generated_command);
	}
	return OK;
}

int aggregate_status(struct buffer *aggregated_status, const struct buffer current_status,
					 const struct buffer new_status, unsigned int device_type) {
	allocate(aggregated_status, new_status.size_in_bytes);
	std::memcpy(aggregated_status->data, new_status.data,
				new_status.size_in_bytes );
	return OK;
}

int aggregate_error(struct buffer *error_message, const struct buffer current_error_message, const struct buffer status,
					unsigned int device_type) {
	return OK;
}

int generate_first_command(struct buffer *default_command, unsigned int device_type) {
	if(device_type == BUTTON_DEVICE_TYPE) {
		pressed_false(default_command);
	} else {
		return NOT_OK;
	}
	return OK;
}

int status_data_valid(const struct buffer status, unsigned int device_type) {
	const char *data = static_cast<char *>(status.data);
	if(data == nullptr) {
		return NOT_OK;
	}
	std::string pressed_false = "{\"pressed\": false}";
	std::string pressed_true = "{\"pressed\": true}";

	switch(device_type) {
		case BUTTON_DEVICE_TYPE:
			if(strncmp(pressed_false.c_str(), data, std::max(status.size_in_bytes, pressed_false.size())) == 0 ||
			   strncmp(pressed_true.c_str(), data, std::max(status.size_in_bytes, pressed_true.size())) == 0) {
				return OK;
			}
			break;
		default:
			break;
	}

	return NOT_OK;
}

int command_data_valid(const struct buffer command, unsigned int device_type) {
	const char *data = static_cast<char *>(command.data);
	if(data == nullptr) {
		return NOT_OK;
	}
	std::string lit_up_false = "{\"lit_up\": false}";
	std::string lit_up_true = "{\"lit_up\": true}";

	switch(device_type) {
		case BUTTON_DEVICE_TYPE:
			if(strncmp(lit_up_false.c_str(), data, std::max(command.size_in_bytes, lit_up_false.size())) == 0 ||
			   strncmp(lit_up_true.c_str(), data, std::max(command.size_in_bytes, lit_up_true.size())) == 0) {
				return OK;
			}
			break;
		default:
			break;
	}
	return NOT_OK;
}