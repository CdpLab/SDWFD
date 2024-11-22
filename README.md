<h1 align="center">Design and Evaluation of a 6-DoF Wearable Fingertip Device for Haptic Shape Rendering</h1>
<div align=center> 
Dapeng Chen, Da Yu, Yi Ding, Haojun Ni, Lifeng Zhu, Hong Zeng, Zhong Wei, Jia Liu, Aiguo Song
  
Nanjing University of Information Science & Technology
</div>
CAD model, bill of materials and detailed manufacturing instructions, as well as its inverse kinematics and control code.The following two images show the CAD structural diagram and prototype of the device.
<div align=center>
<img src="https://github.com/1051138574/Design-and-Evaluation-of-a-6-DoF-Wearable-Fingertip-Device-for-Haptic-Shape-Rendering/blob/main/picture/fig2a.png" alt="Image text" width="250" height="200"/>     <img src="https://github.com/1051138574/Design-and-Evaluation-of-a-6-DoF-Wearable-Fingertip-Device-for-Haptic-Shape-Rendering/blob/main/picture/fig2b3.png" alt="Image text" width="250" height="200"/>
</div>

The CAD structural diagram and prototype of the fingertip haptic device, including (A) mobile platform, (B) rocker arm, (C) fixed plate, (D) stop plate, (E) rigid link, (F) static platform, (G) steering engine, (H) rotating ball head, and (I) thin film pressure sensor.

## The CAD folder contains the assembly and various parts designed by our device.

Static platform: made of resin using 3D printing; Outer circle diameter: 48mm.

Mobile platform: made of resin using 3D printing; Outer circle diameter: 44mm, disc diameter: 22mm.

Rocker arm: Made of resin using 3D printing. Arm length: 13mm. Note: In the CAD folder, arm 2 is used for 3D printing and comes with threads. And rocker arm 1 is only used for CAD display.

Rotating ball head: made of aluminum alloy.

rigid link: made of aluminum alloy.

Screws and nuts: made of aluminum alloy.

Screw post: Made of brass.

Steering engine: Size: 16 × 17 × 6.2 mm³; mass: 17 g; model: SP017A-1; producer: DW Hobby; country: China.

Thin film pressure sensor: diameter: 20 mm; model: RP-C18.3-ST; producer: RunesKee; country: China.


## Installation process

First, install the steeing engines onto the static platform. There are six square holes on the static platform to fix the rectangular prism protruding from the side of the steering engine and enhance its stability. Next, install the copper pillar at the corresponding position on the static platform. Then, install another static platform above the six steering engines, and securely connect two static platforms to the steering engines using screws and nuts. There is a circular hole designed on the static platform for the connection wire of the steering engine to pass through. Subsequently, the mobile platform and rigid link were installed, and the rotating ball head was pre installed in the rigid link. Connect the mobile platform to the rigid link through screws and nuts, with the screws passing through the rotating ball head. The other end of the rigid link is also connected to the rocker arm using this method, while the other end of the rocker arm is connected to the steering engine.

## Code
In the code section, we shared the code for the control, inverse kinematics, steering engine drive module, and membrane pressure sensor of the device. Among them, "FSR" is the code for the thin film pressure sensor, "control" is the code for device control, "inverse" is the code for inverse kinematics, and "pcA9685" is the code for the steering engine drive module.

## Demo video
The demonstration video can be viewed through the following link: https://youtu.be/AHHCAW3EiJQ .



