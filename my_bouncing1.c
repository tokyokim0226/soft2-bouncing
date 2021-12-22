#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

// シミュレーション条件を格納する構造体
// 反発係数CORを追加
typedef struct condition{
  const int width; // 見えている範囲の幅
  const int height; // 見えている範囲の高さ
  const double G; // 重力定数
  const double dt; // シミュレーションの時間幅
  const double cor; // 壁の反発係数
} Condition;

// 個々の物体を表す構造体
typedef struct object{
  double m;
  double x, y;
  double prev_x, prev_y; // 壁からの反発に使用
  double vx, vy;
  double prev_vx, prev_vy;
} Object;

// 授業で用意した関数のプロトタイプ宣言


int my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond);
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond);
void my_update_positions(Object objs[], const size_t numobj, const Condition cond);
void my_bounce(Object objs[], const size_t numobj, const Condition cond);
int is_in_screen(double y, double x, const Condition cond);
int is_inc_or_dec(double a, double b, double c);


int main(int argc, char **argv)
{
  const Condition cond =
    {
     .width  = 75,
     .height = 40,
     .G = 1.0,
     .dt = 0.5,
     .cor = 0.8
    };
  
  size_t objnum = 4;
  Object objects[objnum];

  // objects[1] は巨大な物体を画面外に... 地球のようなものを想定
  objects[0] = (Object){ .m = 60.0, .y = -19.9, .x = -25.0, .vy = 2.0, .vx = 2.0};
  objects[1] = (Object){ .m = 60.0, .y = -19.9, .x = 20.0, .vy = 4.0, .vx = 6.0};
  objects[2] = (Object){ .m = 120.0, .y = -19.9, .x = 30.0, .vy = -3.0, .vx = -3.0};
  objects[3] = (Object){ .m = 100000.0, .y =  1000.0, .x = 0 , .vy = 0.0, .vx = 0};

  // シミュレーション. ループは整数で回しつつ、実数時間も更新する
  const double stop_time = 400;
  double t = 0;
  printf("\n");
  int spacing = 0;
  for (size_t i = 0 ; t <= stop_time ; i++){
    t = i * cond.dt;
    my_update_velocities(objects, objnum, cond);
    my_update_positions(objects, objnum, cond);
    my_bounce(objects, objnum, cond); // 壁があると仮定した場合に壁を跨いでいたら反射させる
    
    // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    spacing += my_plot_objects(objects, objnum, t, cond);
    
    usleep(200 * 1000); // 200 x 1000us = 200 ms ずつ停止
    printf("\e[%dA", spacing); //return cursor to equal amount as spacing
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
        y = objs[i].y + cond.height/2 + 1;
        x = objs[i].x + cond.width/2 + 1;
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
    printf("t = %4.1lf, cor = %0.2lf \r\n", t, cond.cor);
    for (int i=0; i<numobj; i++) {
        printf("obj[%d].y = %6.2lf, objs[%d].x = %6.2lf \r\n", i, objs[i].y, i, objs[i].x);
    }
    
    spacing += numobj + 1;
    return spacing;    
}


void my_update_velocities(Object objs[], const size_t numobj, const Condition cond){
    for (int i = 0; i<numobj; i++){
        for (int j = 0; j < numobj; j++){
            if (i==j) continue;
            double dist = sqrt(pow(objs[i].y - objs[j].y, 2) + pow(objs[i].x - objs[j].x, 2));
            objs[i].prev_vy = objs[i].vy;
            objs[i].vy += cond.G * objs[j].m * (objs[j].y - objs[i].y) / pow(dist, 3) * cond.dt;
            objs[i].prev_vx = objs[i].vx;
            objs[i].vx += cond.G * objs[j].m * (objs[j].x - objs[i].x) / pow(dist, 3) * cond.dt;
        }
    }
}


void my_update_positions(Object objs[], const size_t numobj, const Condition cond){
    for (int i = 0; i < numobj ; ++i){
        objs[i].prev_y = objs[i].y;
        objs[i].y += objs[i].vy *cond.dt;
        objs[i].prev_x = objs[i].x;
        objs[i].x += objs[i].vx * cond.dt;
    }
}


void my_bounce(Object objs[], const size_t numobj, const Condition cond) {
    for (int i = 0; i<numobj; i++) {
        if (is_in_screen(objs[i].prev_y, objs[i].prev_x, cond) != is_in_screen(objs[i].y, objs[i].x, cond)) {

            //think for 4 different cases (all walls)
            if (is_inc_or_dec(objs[i].prev_y, cond.height/2, objs[i].y)) {
                objs[i].y = cond.height/2 - (objs[i].y - cond.height/2) * cond.cor;
                objs[i].vy *= -cond.cor;
            }

            if (is_inc_or_dec(objs[i].prev_y, -cond.height/2, objs[i].y)) {
                objs[i].y = -cond.height/2 + (-objs[i].y - cond.height/2) * cond.cor;
                objs[i].vy *= -cond.cor;
            }

            if (is_inc_or_dec(objs[i].prev_x, cond.width/2, objs[i].x)) {
                objs[i].x = cond.width/2 - (objs[i].x - cond.width/2) * cond.cor;
                objs[i].vx *= -cond.cor;
            }

            if (is_inc_or_dec(objs[i].prev_x, -cond.width/2, objs[i].x)) {
                objs[i].x = -cond.width/2 + (-objs[i].x - cond.width/2) * cond.cor;
                objs[i].vx *= -cond.cor;
            }
        }
    }
}

int is_in_screen(double y, double x, const Condition cond){
    return -cond.height/2 <= y && y <= cond.height/2 && -cond.width/2 <= x && x <= cond.width/2;
}

int is_inc_or_dec(double a, double b, double c) {
    return (a <= b && b <= c) || (c <= b && b <= a);
}