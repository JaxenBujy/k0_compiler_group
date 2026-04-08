/* everything */
fun main() {
    var x: Int = 10;
    var y: Float = 3.14f;

    x += y.toInt();

    if (x >= 10 && y != 0.0f) {
        println("value: " + x);
    }

    val range = 1..10;
    val safe = obj?.field ?: defaultVal;
    val forced = obj!!;
}