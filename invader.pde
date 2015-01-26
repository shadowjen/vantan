PImage img;
PImage img2;
PImage imgz;


void setup(){
    size(500,500);
    img = loadImage("invader.png");
    img2 = loadImage("invader2.png");
}


class enemy{
  int x;
  int size_x;
  
  enemy(int x1,int size_x1){
  x = x1;
  size_x = size_x1;
  }
}
int x = 10;

int size = 20;

int y =20;

int v = 1;

int flag_y = 1;

int link =0;

boolean flag_x = true;

void draw(){
     link++;
     
     background(0);
     
     for(int i =0;i < 10; i++){
     image(img,(x)+img.width*i,y * flag_y);
     }
     x +=v;
     
     if(link%60 == 0){
     imgz = img;
     img = img2;
     img2 = imgz;
     } 
     
     //image(img2,(x-img2.width)/2+50,(y-img2.height)/2);
     
     for(int i=0; i < 10;i++){
       if(x+img.width*(i+1) > 500){
         flag_x = false;
         flag_y +=2;
         v = -v;
     }
     if((x+img.width*i+i) < 0 && flag_x == false){
       flag_x = true;
       flag_y +=2;
       v = -v;
     }
     
     if(flag_y > 20){
       flag_y = 0;
     }
   }
}
