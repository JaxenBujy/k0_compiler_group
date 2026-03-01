// this function tries to define a variable after an executable statement, meaning after the start of the function has ended
fun main(args: Array<String>, arg_count: Int): Int {
    var x = 10;
    var y = 20;
    var z = 30;
    x;
    val a = x + y;
    return 0;
}