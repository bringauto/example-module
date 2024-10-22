# Introduction

<!-- Provide a single sentence or a short paragraph describing the module's purpose. When using the documentation inside repository, add link to the README and vice versa - link the README to this documentation.

Example:

The Test module is provides communication between car test devices and the testing application. For more details on the usage of the module, see the [README](<path-to-readme>).
-->

# Identification and supported devices

<!-- Provide a list of supported devices and their identification.

Example:

The module number/ID: `0`

## Devices

| **Device Name** | **Device Type** | **Device Roles** | Comment                                          |
| --------------- | --------------- | ---------------- | ------------------------------------------------ |
| Test Button 1   | 0               | test-button-1    | Button verifying that the commands can be sent to a device of this module |
| Test Button 2   | 0               | test-button-2    | Button verifying that the commands can be sent to a device of this module |
| Test Camera     | 1               | test-camera      | Camera verifying quality and rate of data transfer from car |
-->

# Messages

<!-- Describe all types of messages handled by the module (including status, status error and command). For each type, describe the message contents, structure and example of the message.

Example of a status description:

## Status

The status message contains information about test components with their state (`ON` or `OFF`).

### Example

``` json
{"state": "ON"}
```
-->

# Details of supported devices

<!-- In this section, list all supported devices. Describe their purpose on the car and their behavior relevant to the application accessing the devices through the module. This includes mainly

- the device state machine,
- how the status machine reflects the state machine's state,
- how the state is affected by the received command.

-->
