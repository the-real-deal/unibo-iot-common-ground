plugins {
    // Apply the java plugin to add support for Java
    java

    // Apply the application plugin to add support for building a CLI application
    // You can run your app via task "run": ./gradlew run
    application

    /*
     * Adds tasks to export a runnable jar.
     * In order to create it, launch the "shadowJar" task.
     * The runnable jar will be found in build/libs/projectname-all.jar
     */
    id("com.gradleup.shadow") version "9.3.1"
}

repositories { // Where to search for dependencies
    mavenCentral()
}

dependencies {
    // Maven dependencies are composed by a group name, a name and a version, separated by colons
    // e.g. implementation("com.omertron:API-OMDB:1.5")
    
    val vertxVersion = "4.2.6"
    // Source: https://mvnrepository.com/artifact/io.vertx/vertx-core
    implementation("io.vertx:vertx-core:$vertxVersion")
    // Source: https://mvnrepository.com/artifact/io.vertx/vertx-web
    implementation("io.vertx:vertx-web:$vertxVersion")
    // Source: https://mvnrepository.com/artifact/io.vertx/vertx-mqtt
    implementation("io.vertx:vertx-mqtt:$vertxVersion")
    // Source: https://mvnrepository.com/artifact/com.google.code.gson/gson
    implementation("com.google.code.gson:gson:2.13.2")
    // Serial library
    implementation("io.github.java-native:jssc:2.10.2")
    /*
     * Simple Logging Facade for Java (SLF4J)
     * See: http://www.slf4j.org/
     */
    val slf4jVersion = "2.0.17"
    implementation("org.slf4j:slf4j-api:$slf4jVersion")
    // Logback backend for SLF4J
    runtimeOnly("ch.qos.logback:logback-classic:1.5.29")

    // JUnit API and testing engine
    testImplementation(platform("org.junit:junit-bom:6.0.2"))
    testImplementation("org.junit.jupiter:junit-jupiter-api")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")
    testRuntimeOnly("org.junit.jupiter:junit-jupiter-engine")
}

application {
    // Define the main class for the application.
    mainClass.set("it.mypckg.sampleapp.AppLauncher")
}

tasks.withType<Test>().configureEach {
    useJUnitPlatform() // Enables the engine of JUnit 5/6
    testLogging { // Additional Options
        // Display all events (test started, succeeded, failed...)
        events(*org.gradle.api.tasks.testing.logging.TestLogEvent.entries.toTypedArray())
        showStandardStreams = true // Show the standard output
    }
}
