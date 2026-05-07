fun loop() {
    var sum: Int = 0;
    for (i in 1..10) {
        sum = sum + i;
        println("sum = $sum");
    }
    println("final sum is $sum");
}