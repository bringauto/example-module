#include <iostream>
#include <condition_variable>
#include <map>
#include <string>
#include <sstream>

#include <fleet_protocol/module_maintainer/external_server/external_server_interface.h>
#include <fleet_protocol/common_headers/device_management.h>
#include <cstring>



static std::string buttonCommand {"{\"lit_up\": false}"};

std::string getId(const ::device_identification &device) {
	std::stringstream ss;
	ss << device.module << "/" << device.device_type << "/"
	   << std::string { static_cast<const char *>(device.device_role.data), device.device_role.size_in_bytes } << "/"
	   << std::string { static_cast<const char *>(device.device_name.data), device.device_name.size_in_bytes };
	return ss.str();
}

struct context {
	std::map<std::string, std::tuple<std::string, struct device_identification>> commandMap;
	bool commandReady{false};
};

void *init(struct config config_data) {
	auto *contextPtr = new context;
	return contextPtr;
}

int destroy(void **context) {
	if(*context == nullptr) {
		return -1;
	}
	auto con = (struct context **)context;
	delete *con;
	*con = nullptr;
	return 0;
}

int wait_for_command(int timeout_time_in_ms, void *context) {
	if(context == nullptr) {
		return -2;
	}
	struct context *con = (struct context *)context;
	std::mutex commandMutex;
	std::unique_lock<std::mutex> lock(commandMutex);
	std::condition_variable commandCondition;
	commandCondition.wait_for(lock, std::chrono::milliseconds(timeout_time_in_ms));
	if(con->commandMap.empty()) {
		return TIMEOUT_OCCURRED;
	}
	printf("[Example module][INFO]: Command is ready\n");
	con->commandReady = true;
	return OK;
}

int pop_command(buffer *command, device_identification *device, void *context) {
	if(context == nullptr) {
		return -1;
	}
	struct context *con = (struct context *)context;

	if(con->commandMap.empty()) {
		printf("[Example module][ERROR]: Command map is empty\n");
		return 0;
	}

	if(not con->commandReady) {
		printf("[Example module][ERROR]: Command is not ready\n");
		return -2;
	}
	con->commandReady = false;
	auto commandTuple = con->commandMap.begin()->second;
	command->size_in_bytes = buttonCommand.size();
	command->data = malloc(buttonCommand.size());
	if(command->data == nullptr) {
		printf("[Example module][ERROR]: Memory allocation error\n");
		return -3;
	}
	std::memcpy(command->data, buttonCommand.data(), buttonCommand.size());


	auto deviceIdentification = std::get<1>(commandTuple);
	device->module = deviceIdentification.module;

	device->device_role.size_in_bytes = deviceIdentification.device_role.size_in_bytes;
	device->device_role.data = malloc(device->device_role.size_in_bytes);
	if(device->device_role.data == nullptr) {
		printf("[Example module][ERROR]: Memory allocation error\n");
		return -3;
	}
	std::memcpy(device->device_role.data, deviceIdentification.device_role.data, device->device_role.size_in_bytes);

	device->device_name.size_in_bytes = deviceIdentification.device_name.size_in_bytes;
	device->device_name.data = malloc(device->device_name.size_in_bytes);
	if(device->device_name.data == nullptr) {
		printf("[Example module][ERROR]: Memory allocation error\n");
		return -3;
	}
	std::memcpy(device->device_name.data, deviceIdentification.device_name.data, device->device_name.size_in_bytes);
	device->device_type = deviceIdentification.device_type;
	device->priority = deviceIdentification.priority;

	printf("[Example module][ERROR]: Command succesfully retrieved from api\n");
	return 1;
}

int forward_status(const buffer device_status, const device_identification device, void *context) {
	printf("[Example module][INFO]: Received status from: %s/%s. Status: %s\n",
		   std::string { static_cast<const char *>(device.device_role.data), device.device_role.size_in_bytes }.c_str(),
		   std::string { static_cast<const char *>(device.device_name.data), device.device_name.size_in_bytes }.c_str(),
		   std::string { static_cast<const char *>(device_status.data), device_status.size_in_bytes }.c_str());
	return 0;
}

int forward_error_message(const buffer error_msg, const device_identification device, void *context) {
	printf("[Example module][INFO]: Received error message from: %s/%s, message: %s\n",
		   std::string { static_cast<const char *>(device.device_role.data), device.device_role.size_in_bytes }.c_str(),
		   std::string { static_cast<const char *>(device.device_name.data), device.device_name.size_in_bytes }.c_str(),
		   std::string { static_cast<const char *>(error_msg.data), error_msg.size_in_bytes }.c_str());
	return 0;
}

int command_ack(const buffer command, const device_identification device, void *context) {
	printf("[Example module][INFO]: Command was successfully delivered\n");
	return 0;
}

int device_connected(const device_identification device, void *context) {
	if(context == nullptr) {
		return -1;
	}
	auto con = (struct context *)context;
	std::string id = getId(device);
	struct device_identification newDevice = {
			.module=device.module,
			.device_type=device.device_type,
			.priority=device.priority
	};


	newDevice.device_role.size_in_bytes = device.device_role.size_in_bytes;
	newDevice.device_role.data = malloc(device.device_role.size_in_bytes);
	if(newDevice.device_role.data == nullptr) {
		printf("[Example module][ERROR]: Memory allocation error\n");
		return -3;
	}
	std::memcpy(newDevice.device_role.data, device.device_role.data, device.device_role.size_in_bytes);

	newDevice.device_name.size_in_bytes = device.device_name.size_in_bytes;
	newDevice.device_name.data = malloc(device.device_name.size_in_bytes);
	if(newDevice.device_name.data == nullptr) {
		printf("[Example module][ERROR]: Memory allocation error\n");
		return -3;
	}
	std::memcpy(newDevice.device_name.data, device.device_name.data, device.device_name.size_in_bytes);
	con->commandMap.insert(std::make_pair(getId(newDevice), std::make_tuple(buttonCommand, newDevice)));
	return 0;
}

int
device_disconnected(const int disconnectType, const device_identification device, void *context) {
	switch(disconnectType) {
		case 0:
			printf("[Example module][INFO]: Device disconnected %s/%s\n",
				   std::string { static_cast<const char *>(device.device_role.data), device.device_role.size_in_bytes }.c_str(),
				   std::string { static_cast<const char *>(device.device_name.data), device.device_name.size_in_bytes }.c_str());
			break;
		case 1:
			printf("[Example module][WARNING]: Device timeout %s/%s\n",
				   std::string { static_cast<const char *>(device.device_role.data), device.device_role.size_in_bytes }.c_str(),
				   std::string { static_cast<const char *>(device.device_name.data), device.device_name.size_in_bytes }.c_str());
			break;
		case 2:
			printf("[Example module][ERROR]: Device error. Disconnected %s/%s\n",
				   std::string { static_cast<const char *>(device.device_role.data), device.device_role.size_in_bytes }.c_str(),
				   std::string { static_cast<const char *>(device.device_name.data), device.device_name.size_in_bytes }.c_str());
			break;
	}
	auto con = (struct context *)context;
	std::string id = getId(device);
	if(not con->commandMap.contains(id)) {
		printf("[Example module][ERROR]: Device is not registered\n");
		return -3;
	}
	con->commandMap.erase(id);
	printf("[Example module][INFO]: Device is disconnected\n");
	return 0;
}
