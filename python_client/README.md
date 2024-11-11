# Internal client examples

This repo contains an implementation of the Example Module using the Internal Client for Fleet Protocol v2.
The devices supported by this module are virtual buttons which send their current state and expect state of their LED as response.

#### Preparing environment

Before running example devices, it is good practice to create a virtual environment:

```bash
python -m venv .venv
source .venv/bin/activate
```

Install dependencies:

```bash
pip install -r requirements.txt
```

## Button

Button is a simple button device with one LED attached to it. As status, it sends JSON with key `"pressed"` indicating whether button is pressed. As command, it expects JSON with key `"lit_up"` indicating the desired state of internal LED.

### Usage

Fake buttons can be run with the `run_buttons.py` script. It expects config in `yaml` format which specifies attributes for a button or multiple buttons (see `buttons.yaml`):

```yaml
- button1_name:
    type: <int> specifying module-specific device type (default is 0)
    role: <str> device role
    priority: <int> device priority (default is 0)

- button2_name: ...
```

To run use:

```bash
python run_buttons.py --ip <server_ip> --port <server_port> --config <path_to_yaml>
```

Multiple fake buttons will be created according to yaml config. They will try to connect to the specified server, generate and send their statuses periodically, and set their LED states according to commands.

> The manual mode of the actual button pressing on the keyboard can be specified by `--manual` argument.
