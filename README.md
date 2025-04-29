# Peltier-Cooling

This is a part of the 2nd round of LAM Research Challenge in which we were tasked to control the power input to the peltiers in order to control the temeperature of water in a copper tumbler according to a specific setpoint which is set using a rotary potentiometer.

In this project, I implement PWM control of the peltier modules in order to control the amount of power supplied to them. This in turn controls the cooling effect produced by the cold side. The heat genearted in the hot side of the peltier is dissipated using a CPU fan. However water cooling the same would be much better and efficient way which I realised later. It is advisable to keep the CPU fan ON after some time even when we disconnect the supply to the peltiers. This should be done to cool down the hot side thus preventing the peltier from getting damaged.Water cooling the hot side is preferred.Selecting appropiate mosfets and buffers with proper current rating is necessary otherwise they may get damaged.

The attached code file and schematic demonstrate the same.
