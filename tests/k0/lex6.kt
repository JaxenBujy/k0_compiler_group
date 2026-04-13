// Global variable (allowed at top level)
val greeting: String = "Welcome to k0";

// Function that calculates area of a circle
fun calculateArea(radius: Double): Double {
    val pi: Double = 3.14159;
    var result: Double;

    result = pi * radius * radius;
    return result;
}

// Main entry point
fun main() {
    // === All variable declarations at the top of the function body ===
    val name: String = "World";
    var count: Int = 3;
    val flag: Boolean = true;
    val radius: Double = 2.5;
    val fruits: Array<String> = arrayOf("apple", "orange", "cherry");
    var maybeMessage: String? = null;
    var i: Int = 1;          // loop variable
    val area: Double;    // declared here, assigned later

    // === Now executable statements begin ===
    println(greeting);
    println("Hello, ${name}!");

    if (flag) {
        println("The flag is true.");
    } else {
        println("The flag is false.");
    }

    // Call another function and assign to pre-declared variable
    area = calculateArea(radius);
    println("Area of circle with radius {radius} is approximately {area}");

    println("Fruits:");
    for (x in 0..count) {
        println(" ${fruits[i]} ");
    }

    count = count + 1;
    println("Count is now: ${count}");

    println("=== Goodbye! ===");
}