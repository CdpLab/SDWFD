<h1 align="center">Design and Evaluation of a 6-DoF Wearable Fingertip Device for Haptic Shape Rendering</h1>
<div align=center> 
Dapeng Chen, Da Yu, Yi Ding, Haojun Ni, Lifeng Zhu, Hong Zeng, Zhong Wei, Jia Liu, Aiguo Song
  
Nanjing University of information Science & Technology
</div>
CAD model, bill of materials and detailed manufacturing instructions, as well as its inverse kinematics and control code.The following two images show the CAD and physical drawings of the device.
<div align=center>
<img src="https://github.com/1051138574/Design-and-Evaluation-of-a-6-DoF-Wearable-Fingertip-Device-for-Haptic-Shape-Rendering/blob/main/picture/fig2a.png" alt="Image text" width="250" height="200"/>     <img src="https://github.com/1051138574/Design-and-Evaluation-of-a-6-DoF-Wearable-Fingertip-Device-for-Haptic-Shape-Rendering/blob/main/picture/fig2b3.png" alt="Image text" width="250" height="200"/>
</div>

The CAD structural diagram and prototype of the fingertip haptic device, including (A) mobile platform, (B) rocker arm, (C) fixed plate, (D) stop plate, (E) rigid linkage, (F) static platform, and (G) servo motor (H) rotating ball joint, (I) thin film pressure sensor.

## The CAD folder contains the assembly and various parts designed by our device.

Static platform: made of resin using 3D printing; Outer circle diameter: 48mm

Dynamic platform: made of resin using 3D printing; Outer circle diameter: 44mm, disc diameter: 22mm

Rocker arm: Made of resin using 3D printing. Arm length: 13mm. Note: In the CAD folder, arm 2 is used for 3D printing and comes with threads. And rocker arm 1 is only used for CAD display.

Rotating ball head: made of aluminum alloy.

Rigid connecting rod: made of aluminum alloy.

Screws and nuts: made of aluminum alloy.

Stud: Made of brass.

Servo motor: Size: 16 × 17 × 6.2 mm³; mass: 17 g; model: SP017A-1; producer: DW Hobby; country: China.

Thin film pressure sensor: diameter: 20 mm; model: RP-C18.3-ST; producer: RunesKee; country: China.


## Installation process

First, install the servo motor onto static platform 1. There are six square holes on static platform 1 to fix the rectangular prism protruding from the side of the servo motor and enhance its stability. Next, install the copper pillar at the corresponding position on static platform 1. Then, install static platform 2 above the six servo motors, and securely connect static platform 1 and static platform 2 to the servo motors using screws and bolts. There is a circular hole designed on static platform 2 for the connection wire of the servo motor to pass through. Subsequently, the mobile platform and rigid connecting rod were installed, and the rotating ball head was pre installed in the rigid connecting rod. Connect the mobile platform to the rigid connecting rod through screws and nuts, with the screws passing through the rotating ball head. The other end of the rigid connecting rod is also connected to the rocker arm using this method, while the other end of the rocker arm is connected to the servo motor.

## Code
In the code section, we shared the code for the control, inverse kinematics, servo motor drive module, and membrane pressure sensor of the device. Among them, FSR is the code for the thin film pressure sensor, control is the code for device control, inverse is the code for inverse kinematics, and pcA9685 is the code for the servo motor drive module.
