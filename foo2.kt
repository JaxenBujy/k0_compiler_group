fun main()
{
   var x = 10;
   var y = 20;
   while (x < y){
      x = x + 1;
   }

   println("x before $x");
   x = x + y;
   println("x after $x");

   return 0;
}