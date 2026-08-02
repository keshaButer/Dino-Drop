plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "com.kesha.dinodrop"
    compileSdk = 36

    defaultConfig {
        applicationId = "com.kesha.dinodrop"
        minSdk = 24
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"

        ndk {
            abiFilters.clear()
            abiFilters.add("arm64-v8a")
        }

        externalNativeBuild {
            cmake {
                cppFlags += listOf("-std=c++17")
            }
        }

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
    }

    signingConfigs {
        create("release") {
            storeFile = file("dinodrop.keystore")
            storePassword = "Glebas2016"
            keyAlias = "dinodrop_alias"
            keyPassword = "Glebas2016"
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = true
            isShrinkResources = true

            signingConfig = signingConfigs.getByName("release")

            externalNativeBuild {
                cmake {
                    arguments("-DCMAKE_BUILD_TYPE=Release")
                }
            }

            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    lint {
        checkReleaseBuilds = false
        abortOnError = false
    }

    buildFeatures {
        viewBinding = false
    }

    androidResources {
        noCompress += "png"
    }
}

// dependencies {
//     implementation(libs.appcompat)
//     implementation(libs.constraintlayout)
//     implementation(libs.material)
//     testImplementation(libs.junit)
//     androidTestImplementation(libs.espresso.core)
//     androidTestImplementation(libs.ext.junit)
// }

dependencies {
    implementation(libs.appcompat)
}
