package com.thinking.videoinput;

import android.media.MediaMetadataRetriever;
import android.util.Log;

/**
 * Created by yong on 2017/3/1.
 */

public class MediaHandler {

    private static MediaHandler thiz;
    private MediaMetadataRetriever mRetriever;
    private long mDuration = -1;
    private int mFps;
    private boolean isStop = false;
    private BaseMediaView mView;

    private Boolean isNeedWait = false;

    private Thread mGetFrameThread = new Thread() {
        private long mStartPoint = 0;
        private long mStopPoint = 0;
        private long mFrameTime;

        @Override
        public void run() {
            super.run();
            //Log.i("yuyong", "mDuration-->" + mDuration);
            mStopPoint = mDuration;
            mStartPoint = 0;
            mFrameTime = mFps;
            while (isStop) {
                //Log.i("yuyong", "mStartPoint-->" + mStartPoint + " mStopPoint-->" + mStopPoint);
                isNeedWait = true;
                if (mView == null)
                    break;
                mView.onGetFrame(mRetriever.getFrameAtTime(mStartPoint * 1000, MediaMetadataRetriever.OPTION_CLOSEST));
                mStartPoint += mFrameTime;
                if (mStartPoint > mStopPoint) {
                    break;
                }
                if (isNeedWait) {
                    synchronized (isNeedWait) {
                        try {
                            Thread.sleep(mFrameTime);
                            isNeedWait.wait();
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }
    };

    public void doContinue() {
        isNeedWait = false;
        synchronized (isNeedWait) {
            isNeedWait.notify();
        }
    }

    private MediaHandler() {
        mRetriever = new MediaMetadataRetriever();
    }

    public static MediaHandler getThiz() {
        if (thiz == null)
            thiz = new MediaHandler();
        return thiz;
    }

    public void setView(MediaView v) {
        mView = v;
    }

    public void getVideoFrame(String file_path) {
        mRetriever.setDataSource(file_path);
        mDuration = Long.parseLong(mRetriever.extractMetadata(MediaMetadataRetriever.METADATA_KEY_DURATION));//时长，微秒
        Log.i("yuyong", "fps-->" + mRetriever.extractMetadata(MediaMetadataRetriever.METADATA_KEY_CAPTURE_FRAMERATE));
        //mFps = Integer.parseInt(mRetriever.extractMetadata(MediaMetadataRetriever.METADATA_KEY_CAPTURE_FRAMERATE));//帧率
        mFps = 40;
        isStop = true;
        mGetFrameThread.start();
    }

    public void stop() {
        isStop = true;
        mView = null;
    }

}
