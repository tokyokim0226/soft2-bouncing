# **Padding in struct_alignment.c**

1. Compile:
```
gcc -o struct_alignment struct_alignment.c
```

2. Execute code
```
./struct_alignment
```

Once this is done, we get the following result:
```
address
           s_end               s3               s2               s1          s_begin
  0x7ffd8039acb0   0x7ffd8039ad30   0x7ffd8039adb0   0x7ffd8039ae40   0x7ffd8039aec0

sizeof
         student  tagged_student1  tagged_student2
             128              136              128

   s_end: 0x7ffd8039acb0 0x7ffd8039acb4 0x7ffd8039acb8 0x7ffd8039ad1c 0x7ffd8039ad20 0x7ffd8039ad28
      s3: 0x7ffd8039ad30 0x7ffd8039ad34 0x7ffd8039ad38 0x7ffd8039ad9c 0x7ffd8039ada0 0x7ffd8039ada8
      s2: 0x7ffd8039adb0 0x7ffd8039adb4 0x7ffd8039ae18 0x7ffd8039ae20 0x7ffd8039ae28 0x7ffd8039ae30
      s1: 0x7ffd8039ae40 0x7ffd8039ae44 0x7ffd8039aea8 0x7ffd8039aeb0 0x7ffd8039aeb8
 s_begin: 0x7ffd8039aec0 0x7ffd8039aec4 0x7ffd8039af28 0x7ffd8039af30 0x7ffd8039af38
 ```

Looking at how each struct student, tagged_student1, tagged_student2 are constructed, they should each have the following number of bytes:

- student: 124 bytes
- tagged_student1: 125 bytes
- tagged_student2: 125 bytes

But in reality, as seen by our output, they each have:

- student: 128 bytes (+ 4 bytes)
- tagged_student1: 136 bytes (+ 11 bytes)
- tagged_student2: 128 bytes (+ 3 bytes)

We can see that they respectively have a padding of 4, 11, and 3 bytes. Let's see why this is the case:


## **struct student**
-----
If we look at the 3rd item of `s_begin ` that is from `struct student`, we may expect `int age` to have 4 bytes allocated to it because it is an `int` type. However, there are 8 bytes allocated to the 3rd item, and this is due to the 4 bytes of padding `(0x7ffd8039af2c - 0x7ffd8039af2f)` that occurs so that it can match with the 4th item that is a type `double`. As a result, `struct student`, which we may expect to only occupy 


## **struct tagged_student1**
-----

When we look at `s2`, similarly as in `struct student`, 4 bytes of padding occurs `(0x7ffd8039aeac - 0x7ffd8039aeaf)` in order to match with the 4th item that is a type `double`. However, in this case, there is an additional 7 bytes of padding that occurs after item 6 that is a type `char`, in order to match the size of rendering of the program to 8 bytes (that of `double`). We can see this as the address of the first item of `s1` `(0x7ffd8039ae40)` is 8 bytes after the address of the last item of `s2` `(0x7ffd8039ae30)`, and so we know that the 7 bytes in the address range `0x7ffd8039ae31 - 0x7ffd8039ae37` is filled due to padding. As a result, there is a total of 4 + 7 = 11 bytes of padding that occurs in this case, making `struct tagged_student1` occupy 136 instead of the expected 125.


## **struct tagged_student2**
-----
Looking at s_end, the first item is a `char`, and the second item is a type `int`. Here, there are 3 bytes of padding `(0x7ffd8039acb1 - 0x7ffd8039acb3)` that occurs after the first item of s_end to match with the number of bytes that the second item `(int)` takes up.
