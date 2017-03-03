package com.thinking.skindetection;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

/**
 * Created by Yu Yong on 2017/2/23.
 */
public class FileTools {
    public static boolean writeAssetsToSd(String file_path_name, Context context, String file_abs_path) {
        File obj_file = new File(file_abs_path);
        try {
            if (obj_file.exists())
                return true;
            if (!obj_file.getParentFile().exists()) {
                obj_file.getParentFile().mkdirs();
            }
            obj_file.createNewFile();
            InputStream inputStream = context.getAssets()
                    .open(file_path_name);
            FileOutputStream fout = new FileOutputStream(obj_file);
            byte[] buffer = new byte[1024 * 10];
            while (true) {
                int len = inputStream.read(buffer);
                if (len == -1) {
                    break;
                }
                fout.write(buffer, 0, len);
            }
            inputStream.close();
            fout.close();
            return true;
        } catch (Exception e) {
            String msg = e.getMessage();
            Log.e("yuyong", msg);
            return false;
        }
    }
}
