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
            if (checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE), 1)
            }
        }

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.btnOpen.setOnClickListener {
            Thread {
                Log.d(TAG, "连接相机")
                val rtspUrl =
//                    "rtsp://admin:123456@58.56.152.66:8073/stream0"
                    "rtsp://admin:123456@192.168.31.46:3389/stream0"
                open(rtspUrl)
            }.start()
        }

        binding.btnClose.setOnClickListener {
            Log.d(TAG, "关闭相机")
            close()
        }

        binding.btnStart.setOnClickListener {
            val pathName = "/sdcard/${SystemClock.elapsedRealtime()}.h264"
            start(pathName)
        }

        binding.btnStop.setOnClickListener {
            stop()
        }
    }

    private external fun open(rtspUrl: String)
    private external fun start(pathName: String)

    external fun stop()
    external fun close()

    companion object {
        const val TAG = "MainActivity"

        // Used to load the 'ffmpeg' library on application startup.
        init {
            System.loadLibrary("ffmpeg-test")
        }
    }
}