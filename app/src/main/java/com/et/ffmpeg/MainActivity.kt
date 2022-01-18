package com.et.ffmpeg

import android.Manifest
import android.annotation.SuppressLint
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.os.SystemClock
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.et.ffmpeg.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    @SuppressLint("AuthLeak")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED
        }
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.btnStart.setOnClickListener {
            Thread {
                Log.d(TAG, "开始录制")
                val rtspUrl =
                    "rtsp://admin:123456@192.168.31.46:3389/cam/realmonitor?channel=1&subtype=1"
                val pathName = "/sdcard/${SystemClock.elapsedRealtime()}.h264"
                play(rtspUrl, pathName)
            }.start()
        }

        binding.btnStop.setOnClickListener {
            Log.d(TAG, "结束录制")
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
        const val TAG = "MainActivity"

        // Used to load the 'ffmpeg' library on application startup.
        init {
            System.loadLibrary("ffmpeg-test")
        }
    }
}