fun main() {
    var x: Int = 5;
    var y: Int = 3;
    var z: Int = add(x,y);

    println("z: $z");

    if (x > y) {
        println("x is bigger than y");
    }
    else {
        println("x is smaller than y");
    }

    while (x < 10) {
        println("x: $x");
        x = x + 1;
    }
}

fun add(num1: Int, num2: Int): Int {
    return num1 + num2;
}