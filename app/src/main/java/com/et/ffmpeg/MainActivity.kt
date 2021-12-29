package com.et.ffmpeg

import android.annotation.SuppressLint
import android.os.Bundle
import android.os.SystemClock
import androidx.appcompat.app.AppCompatActivity
import com.et.ffmpeg.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    @SuppressLint("AuthLeak")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.btnStart.setOnClickListener {
            Thread{
                val rtspUrl = "rtsp://admin:123456@192.168.1.10:554/cam/realmonitor?channel=1&subtype=1"
                val pathName = "/sdcard/${SystemClock.elapsedRealtime()}.h264"
                play(rtspUrl, pathName)
            }.start()

        }

        binding.btnStop.setOnClickListener {
            stop()
        }
    }

    /**
     * A native method that is implemented by the 'ffmpeg' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    private external fun play(rtspUrl: String, pathName: String)

    external fun stop()

    companion object {
        // Used to load the 'ffmpeg' library on application startup.
        init {
            System.loadLibrary("ffmpeg-test")
        }
    }
}