# Bootstrap Fan

The Bootstrap Fan Component is identical to the [Speed Fan Component](https://esphome.io/components/fan/speed.html) except it add new configuration variables to control bootstrapping a fan.  This will ensure that the **output** is set to an **initial_value** for a **duration** when transitioning from the *off* to *on* state.  This is particularly useful for helping a fan motor to start.

The component also supports specifying a **min_value** and a **max_value**, which is used to clamp the value whilst the fan is on and after the initial boot. That is any value after booting will be coerced to fall between the minimum and maximum.

*NB* Therefore, the value sent to the **output** may differ to that currently set during booting, and when being constrained by the minimum and maximum.  To see the actual ouput value you will need to add a sensor or similar.

```yaml
# example configuration entry
fan:
  - platform: bootstrap
    name: Bootstrap fan
    output: gpio_d1
    initial_value: 80
    duration: 5s
    min_value: 30

output:
  - platform: gpio
    pin: D1
    id: gpio_d1
```

## Configuration Variables

* **initial_value** *(Optional, int)*: The fan speed to set when turning on the fan.  Defaults to [**speed_count**](https://esphome.io/components/fan/speed.html).
* **duration** *(Optional, [Time](https://esphome.io/guides/configuration-types#config-time))*: The amount of time to set the fan to the **initial_value**.  Setting this value to `0ms` will effectively disable bootstrap functionality, which is useful if you just want to take advantage of the other options. Defaults to 1s.
* **min_value** *(Optional, int)*: Whilst the fan is on, and after boot, any value below the minimum is coerced to the minimum. Defaults to 0.
* **max_value** *(Optional, int)*: Whilst the fan is on, and after boot, any value above the maximum is coerced to the maximum. Defaults to 0.
* **use_higher** *(Optional, boolean)*: When `true`, if the **initial_value** is below the requested speed, when turning the fan on, the requested speed is used instead of the **initial_value**; otherwise, the **initial_value** is always used during the boot **duration**.  For example, in the above configuration switching the fan on with a speed of 100 will use that speed during the boot process, rather than `80`; however, if **use_higher** is `false` then is would still boot at 80, before increasing to `100` after 5s. Defaults to `true`.
* All other options from [Speed Fan Component](https://esphome.io/components/fan/speed.html)
