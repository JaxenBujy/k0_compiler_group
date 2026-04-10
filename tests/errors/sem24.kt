// Expected error: argument 1 of greet is nullable but parameter expects non‑nullable
fun greet(msg: String) {
    println(msg);
}

fun main() {
    var m: String? = "hello";
    greet(m);
}