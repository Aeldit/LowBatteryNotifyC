# LowBatteryNotifyC

Sends a notification to the user's desktop when battery percentage drops bellow `20`, `10` and `5` %.

This program runs in the background but consumes only between `1.2` and
`1.3` MB of RAM,
which means that it has a low impact on the battery. Moreover, it checks for battery percentage only once every minute

## Dependencies
Uses the `notify-send` command (see https://gitlab.gnome.org/GNOME/libnotify)

## Usage

To use this program, make it start at the launch of your session / computer

On Hyprland for example, you would add the following line to you `hyprland.conf` file:

```conf
exec-once = path/to/low-battery-notifier
```

## Build

To build the project, use the following commands:

```sh
git clone https://github.com/Aeldit/LowBatteryNotify.git
cd LowBatteryNotifyC
make # builds the project
```

The file will then be located at `./low-battery-notifier`

