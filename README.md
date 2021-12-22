# **Software II Week I 課題 - Bouncing**


# Introduction

- [課題１](#課題１) - Makes sure that the ball bounces off the walls. Runs even when there are more than 3 objects in the screen. The default is set to have 4 objects, 3 in the screen and one big object outside of the screen to make it like the objects on the screen are affected by gravity.

- [課題２](#課題２) - The initialization for the objects can be done through a file (in a .dat format). 


    **！！！課題１と課題２の注意点！！！**

    Updating the velocities, positions and checking for whether the ball should bounce off the walls depend on `dt` - the time interval to observe. If not small enough `dt` is chosen, sometimes the code may bug out. 

- [課題３](#課題３) - Objects that get close enough (I have currently manually set it to distance < 2), they will fuse into one object. An extra example file to quickly see this take action is added. Conservation of momentum for when the objects are made into one is written down, but may not function as accurately for when `dt` is set inappropriately (written below)

    **！！！課題3の注意点！！！**

    `dt` is also a crucial determining factor here, as when `dt` is not small enough, the computational approximation for the objects may not be a very good representation for when their movements are continuous.

- [課題４](#課題４) - Solar system simulator - It will show Mercury, Venus, Earth, and Mars (the gas giants are too far away to be shown to scale)

    [![image.png](https://i.postimg.cc/Ssvhgn8K/image.png)](https://postimg.cc/jwPm5dMV)


# 課題１

Compile:

```
gcc -o my_bouncing1 my_bouncing1.c -lm
```

Execute:

```
./my_bouncing1
```

Three balls will be bouncing off of walls, and it will seem like they are under gravity. Moreover, the gravitational effect between the objects on the screen will be visible whenever they come together very closely.

## Brief Explanation for Code
-----

I have defined the following functions in order to run the program:

- `typedef struct condition{}Condition` - is a class that is used to initialize the conditions (size (height and width) of the board and constant variables such as G, dt, and cor)

- `typedef struct object{}Object` - used to create an object with the object's mass, positions, and velocities.

- `int my_plot_objects` - plotting the objects out inside a square grid, where the height and width are given as arguments (defined in `main()`)

- `void my_update_velocities` - updates the velocity accordingly using the distance between an object and all the other objects, their masses, and the constant G.

- `void my_update_positions` - updates the positions of the objects using the object's velocity (uses `prev_x` and `prev_y` which are stored when updating for velocity)

- `void my_bounce` - makes it so that the object bounces on walls. It is done by checking whether the object's next location (for the next `dt`) is outside of the given grid (in other words, not plottable using `my_plot_objects`). If that is the case, then use the coefficient `cor` (反発係数) to make the object bounce back from the wall with a lower velocity. Applied on all four walls.

- `int is_in_screen` - checks for whether object is in the screen - used in `void my_bounce`

- `int is_inc_or_dec` - checks for whether the function is an monotonically increasing or decreasing function


Inside the `main()`, the initial conditions are set, the number of objects are declared, and `objects[objnum]`, a class of `Objects`, is made.

Each object's mass, position, and velocities are declared manually, and then while keeping track of the time, the code will continue looping through updating the velocities/positions and printing out the output until the set stop_time is reached.

With the current code, there will be 4 objects - 3 visible in the screen and 1 outside the screen, acting like the force of gravity on the 3 objects. The 3 objects will bounce around within the given box.

However, it must be noted that if the objects come too close and `dt` is too big, then `my_bounce` may not work properly. This is due to the computational approximation used in this code.

# 課題２

Compile:

```
gcc -o my_bouncing2 my_bouncing2.c -lm
```

Execute:

- test1.dat --> 基本的なobject１個と地球

- test2.dat --> my_bouncing1.c に初期化されているobjectsと同じ情報が記述されているファイル - `./my_bouncing1`の出力と同じく出る

```
./my_bouncing2 3 test2.dat
```

`./my_bouncing2 [object個数]　[ファイル名]` の形。

もしobject個数がファイルに書かれているobject個数より:

- 少ない場合：入力した個数の分だけファイルから呼び込む
- 多い場合: 足りない分だけランダムな位置にランダムな初期値を持つobjectを作る


## Additions made to 課題１
-----

The basis for `my_bouncing2.c` is `my_bouncing1.c`, with only a few slight changes:

- Addition of the function `read_objects` - this function is one that takes in a file input and scans through the file to read the data of objects stored within the file. It will read files in `.dat` format, which will be in the following format:

```
# m y x vy vx
```

For instance, the file `test2.dat` looks like the following:

```
# m y x vy vx
60.0 -19.9 -25.0 2.0 2.0 # 小さい玉1
60.0 -19.9 20.0 4.0 6.0 #小さい玉2
120.0 -19.9 30.0 -3.0 -3.0 #小さい玉2
100000.0 1000.0 0.0 0.0 0.0 # 地球
```

which are the same objects as the ones used in 課題１. Hence, the output looks the same.

This read_objects is used once before the loop is initiated in `main()`


# 課題３

Compile:

```
gcc -o my_bouncing3 my_bouncing3.c -lm
```

Execute:


```
./my_bouncing3 3 test2.dat
```

## Additions made to 課題２
-----

- Defining an extra function `void fuse_objects` - This function makes it so that when two objects are close than a certain distance (currently manually set to 2), they will fuse into one object. Here, conservation of momentum is realized.


# 課題４
# Solar System Simulator

[![image.png](https://i.postimg.cc/pXzgqrPV/image.png)](https://postimg.cc/PPf3qtL0)  [![image.png](https://i.postimg.cc/Ssvhgn8K/image.png)](https://postimg.cc/jwPm5dMV)


I have made a simple solar system simulation model, where we assume planets revolve around the sun on a 2D plane (in reality, the planets, especially the gas giants, do not all revolve around the sun on a flat plane). In this simulator, 4 planets are shown to revolve around the sun: the gas giants (Jupiter, Saturn, Uranus, and Neptune) being too far away from the sun, so those 4 planets are not shown on the screen. 

To make this simulator possible, I have gathered the following information:

- Location of the planets as of 2021/December/22/15:33:35 from the following websites:

    1. [Heliocentric Coordinates of Planets](https://eco.mtk.nao.ac.jp/cgi-bin/koyomi/cande/planet_ecliptic_en.cgi) - Heliocentric coordinates of planets and their distances (in au)
    2. [Solar System Calculator - by Don Cross](http://cosinekitty.com/solar_system.html) - Earth's heliocentric coordinates and distance (in au)
    3. [Planetary Fact Sheet](https://nssdc.gsfc.nasa.gov/planetary/factsheet/) - mass of planets and gravitational coefficient
    4. [The Planets Today.Com](https://www.theplanetstoday.com/) - To visualise the actual current location of planets


Using the location of the planets as of 2021/December/22/15:33:35 as the initial location of planets, the planets will rotate around the the Sun, which will not move much because of its relatively huge mass.

Here is how to compile and execute:

Compile: 

```
gcc -o my_bouncing4 my_bouncing4.c -lm
```

Execute:

```
./my_bouncing4 5 solar.dat
```

The format is `./my_bouncing4 [number of celestial bodies you want to print] [file name]`

If you try to print more than 5 celestial bodies (for instance: `./my_bouncing4 6 solar.dat`), you will be returned an error saying that there are only 5 celestial objects max. 

If you try to print less than 5, then simply you will have n-1 planets and the sun on the screen.

With the correct command, an output that looks something like this will show, with visibly 4 planets orbiting around the Sun (using the execution above):

[![image.png](https://i.postimg.cc/bNfKPZb4/image.png)](https://postimg.cc/7bXBMYKN)


## Brief Explanation of code
-----

First, a solar.dat file is read, which stores the data of planets in the following format:

```
#mass, current radial location, au, degrees per day
1.9891e30 0 0 0 # 太陽
3.301e23 316.51912 0.4150455 3.48376 # 水星
4.869e24 80.62214 0.7202599 1.62214 # 金星
5.972e24 90.0 0.9836958 0.95456 # 地球
6.417e23 231.21184 1.5485432 0.50515 # 火星

```

This file is read, and according to the planets, mass, radial location, distance (au), and radial velocity (in degrees per day), we calculate where the planets would be on a scaled-down Cartesian x-y plane. The velocities for each planet is also calculated in a similar manner, and we make sure that the gravitational force between the planets (primarily the Sun, due to its huge mass) affects their orbit.

When reading the file, I had to make sure that the information of the planets were succesfully translated onto the x-y plane. To do this, the following code was used within the `read_objects` function:

```
double radius, degree, velocity; //there are the initial values that will be read in from the file
sscanf(buffer, "%lf %lf %lf %lf", &objs[i].m, &degree, &radius, &velocity);
double distance = radius * cond.au; // calculate the distance in meters (radius is expressed in au)
double radians = degree / 360 * 2 * M_PI;
double radial_velocity = velocity / 360 * 2 * M_PI;

objs[i].x = distance * cos(radians);
objs[i].y = distance * -sin(radians);

objs[i].vx = distance * radial_velocity * -sin(radians) / (60*60*24);
objs[i].vy = distance * radial_velocity * -cos(radians) / (60*60*24);
```

- The x-y locations of the planets were determined using their distance from the sun and their currrent angle displacement from the sun (`cos` for the x axis and `-sin` for the y axis)

- The velocities of the planets were determined by first retrieving the radial velocities of the planets in radians, then converting them to cartesian velocity (`-cos` for vx and `-sin` for vy)

Then, once the locations of the planets in the x-y plane was determined, I made sure to scale them down so that they could be represented on the screen. Within the `my_plot_objects` function, it can be seen that the x axis is elongated by double that of the y-axis, because in the terminal the x is shorter than the y ( to prevent representation of the planets orbiting in a oval/non-circular manner). Also, the time was adjusted to represent not simply the number of seconds but the number of days elapsed. As will be explained below, there are several reasons why Earth's orbit may not be exactly 365 days in this simulation.

Within `main()`, the initial conditions within `Condition cond` were updated to work for this solar system model.


While this is an interesting model, there are indeed limitations:

1. The planets' speed (degrees per day) actually changes according to its location. This simulation does not take that into account, and we can visibly observe the Earth completing 1 full cycle in less than 365 days in the simulation (around 320 days)

2. As mentioned, we assume the planets are all on a flat surface, which in reality, they are not.

3. Only the Sun's effect of gravitation and the four closest planets to the sun have a force between each other. In this model, there are no other celestial bodies (asteroids, comets, gas giants, etc.), and hence the force acting on the planets are not as accurate, which also is a factor that visibly impacts Earth's cycle in the simulation.


