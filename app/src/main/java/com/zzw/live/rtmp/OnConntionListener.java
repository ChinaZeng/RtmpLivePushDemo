package com.zzw.live.rtmp;

public interface OnConntionListener {

    void onConntecting();

    void onConntectSuccess();

    void onConntectFail(String msg);
}
