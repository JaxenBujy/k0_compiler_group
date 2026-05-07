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

    loop();
}

fun add(num1: Int, num2: Int): Int {
    return num1 + num2;
}

fun loop() {
    var sum: Int = 0;
    for (i in 1..10) {
        sum = sum + i;
        println("sum = $sum");
    }
    println("final sum is $sum");
}