// Expected error: for-loop range must be whole number numeric types
fun main() {
    for (i in 1.0..5.0) {
        println(i);
    }
}