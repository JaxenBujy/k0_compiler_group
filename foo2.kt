fun main()
{
   var x = 10;
   var y = 20;
   while (x < y){
      x = x + 1;
   }

   jeg(x,y);

   return 0;
}

fun jeg(x: Int, y: Int) {
   x = y + x;
   return x;
}