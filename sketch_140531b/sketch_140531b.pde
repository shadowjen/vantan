int level = 0;


void setup()
{
  size(600,600);
  colorMode(HSB);
}

float rth = -PI/4;
float lth = PI/4;


void tree(float d){
  if(d < 3){
    return;
}
stroke(map(level,0,10,0,255) % 256,255,255);
line(0,0,0,d);
level++;
translate(0,d);
rotate(rth);
tree(0.7*d);
rotate(-rth);
rotate(lth);
tree(0.8*d);
rotate(-lth);
translate(0,-d);
level--;
}

void draw()
{
 background(255);
 translate(width/2,height);
 rotate(-PI);
 rth = map(mouseX,0,width-1,PI/4,-PI/4);
 lth = map(mouseY,0,height-1,PI/4,-PI/4);
 tree(50);
}

