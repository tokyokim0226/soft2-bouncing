#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

//condition updated to include au (the distance away from the sun)
typedef struct condition{
  const int width; // 見えている範囲の幅
  const int height; // 見えている範囲の高さ
  const double G; // 重力定数
  const double dt; // シミュレーションの時間幅
  const double au;
} Condition;

// 個々の物体を表す構造体
typedef struct object{
  double m;
  double x, y;
  double prev_x, prev_y; // 壁からの反発に使用
  double vx, vy;
} Object;

// 授業で用意した関数のプロトタイプ宣言


int my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond);
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond);
void my_update_positions(Object objs[], const size_t numobj, const Condition cond);
int is_in_screen(double y, double x, const Condition cond);
int is_inc_or_dec(double a, double b, double c);
void read_objects(int numobj, Object objs[], char filename[], const Condition cond);
double distance_between_objects(Object object1, Object object2, const Condition cond);


int main(int argc, char **argv){
    const Condition cond =
        {
        .width  = 75,
        .height = 40,
        .G = 6.67430e-11,
        .dt = 1 * 60 * 60 * 24,
        .au = 149597870700
        };

    if (argc != 3) {
        fprintf(stderr, "Usage: ./my_bouncing2 <num_of_objects> <filename>\n");
        return EXIT_FAILURE;
    }


    size_t objnum = atoi(argv[1]);
    Object objects[objnum];

  // ファイルからオブジェクトを呼び込む
    read_objects(objnum, objects, argv[2], cond);


  // シミュレーション. ループは整数で回しつつ、実数時間も更新する
    const double stop_time = 400 * 60 * 60 * 24;
    double t = 0;
    printf("\n");
    int spacing = 0;
    for (size_t i = 0 ; t <= stop_time ; i++){
        t = i * cond.dt;
        my_update_velocities(objects, objnum, cond);
        my_update_positions(objects, objnum, cond);

        // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
        spacing += my_plot_objects(objects, objnum, t, cond);

        usleep(100 * 1000); // 200 x 1000us = 200 ms ずつ停止
        printf("\e[%dA", spacing);
        spacing = 0;
    }
    return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト


int my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond) {
    
    char board[cond.height+2][cond.width+2];

    //Fill in the board with all empty spaces first
    for (int y = 0; y<cond.height+2 ;y++) {
        for (int x = 0; x<cond.width+2; x++) {
            board[y][x] = ' ';
        }
    }

    //物体
    for (int i = 0 ; i<numobj; i++) {
        int y, x;
        y = objs[i].y / (cond.au * 0.1) + cond.height/2 + 1;
        x = objs[i].x / (cond.au * 0.05) + cond.width/2 + 1;
        //If the object is within the boundaries of the board, then print baord
        if (0 < x && 0 < y && x < cond.width+1 && y < cond.width+1) {
            board[y][x] = 'o';
        }
    }


    board[0][0] = board[0][cond.width+1] = board[cond.height+1][0] = board[cond.height+1][cond.width+1] = '+';

    for (int x= 1; x < cond.width+1; x++) {
        board[0][x] = '-';
        board[cond.height+1][x] = '-';
    }

    for (int y = 1; y < cond.height+1; y++) {
        board[y][0] = '|';
        board[y][cond.width+1] = '|';
    }

    //printint out the board
    for (int y = 0; y < cond.height+2; y++) {
        for (int x = 0; x < cond.width +2; x++) {
            printf("%c",board[y][x]);
        }
        printf("\r\n");
    }

    int spacing = cond.height+2;

    //printint out information
    printf("days = %4.1lf \r\n", t/(60*60*24));
    for (int i=0; i<numobj; i++) {
        printf("[%d]  .y = %6.2lf  .x = %6.2lf (units: au)   .vy = %6.3lf  .vx = %6.3lf [units: au/day] \r\n",
        i, objs[i].y / cond.au, objs[i].x / cond.au, objs[i].vy / cond.au * (60*60*24), objs[i].vx / cond.au * (60*60*24));
        spacing ++;
    }

    return spacing +1;    
}

//getting the velocity of objects - DONE
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond){
    for (int i = 0; i<numobj; i++){
        for (int j = 0; j < numobj; j++){
            if (i==j) continue;
            double dist = distance_between_objects(objs[i], objs[j], cond);
            objs[i].vy += cond.G * objs[j].m * (objs[j].y - objs[i].y) / pow(dist, 3) * cond.dt;
            objs[i].vx += cond.G * objs[j].m * (objs[j].x - objs[i].x) / pow(dist, 3) * cond.dt;
        }
    }
}

//getting positions of objects - DONE
void my_update_positions(Object objs[], const size_t numobj, const Condition cond){
    for (int i = 0; i < numobj ; ++i){
        objs[i].prev_y = objs[i].y;
        objs[i].y += objs[i].vy *cond.dt;
        objs[i].prev_x = objs[i].x;
        objs[i].x += objs[i].vx * cond.dt;
    }
}

//checking for whether object is in screen - DONE
int is_in_screen(double y, double x, const Condition cond){
    return -cond.height/2 <= y && y <= cond.height/2 && -cond.width/2 <= x && x <= cond.width/2;
}

//checking for whether the numbers are monotonically increasing or decreasing (checks in order) - DONE
int is_inc_or_dec(double a, double b, double c) {
    return (a <= b && b <= c) || (c <= b && b <= a);
}

//reading in from a file - DONE
void read_objects(int numobj, Object objs[], char filename[], const Condition cond) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Could not open file: \"%s\" \n", filename);
        exit(1);
    }

    int buffer_length = 1000;
    char buffer[buffer_length];
    int i = 0;
    while (i < numobj && (fgets(buffer, buffer_length-1, fp)) != NULL) {
        
        if (buffer[0] == '#'){
            continue;
        }

        double radius, degree, velocity; //there are the initial values that will be read in from the file
        sscanf(buffer, "%lf %lf %lf %lf", &objs[i].m, &degree, &radius, &velocity);
        double distance = radius * cond.au; // calculate the distance in meters (radius is expressed in au)
        double radians = degree / 360 * 2 * M_PI;
        double radial_velocity = velocity / 360 * 2 * M_PI;

        objs[i].x = distance * cos(radians);
        objs[i].y = distance * -sin(radians);

        objs[i].vx = distance * radial_velocity * -sin(radians) / (60*60*24);
        objs[i].vy = distance * radial_velocity * -cos(radians) / (60*60*24);

        ++i;
    }

    srand(time(0));
    //if there are not enough, then fill in objects randomly
    while (i < numobj) {
        objs[i].m = (double)rand() / RAND_MAX * 40 + 40;
        objs[i].x = (double)rand() / RAND_MAX * cond.width - cond.width / 2;
        objs[i].y = (double)rand() / RAND_MAX * cond.height - cond.height / 2;
        objs[i].vx = (double)rand() / RAND_MAX * 20 - 10;
        objs[i].vy = (double)rand() / RAND_MAX * 20 - 10;
        i++;
    }

    fclose(fp);
    
}

//calculating distances between objects - DONE
double distance_between_objects(Object object1, Object object2, const Condition cond){
    return sqrt(pow(object1.y-object2.y, 2) + pow(object1.x - object2.x, 2));
}