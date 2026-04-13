// Expected error: when expression not supported in k0
fun main() {
    val x: Int = 2;
    when (x) {
        1 -> println("one");
        2 -> println("two");
    }
}