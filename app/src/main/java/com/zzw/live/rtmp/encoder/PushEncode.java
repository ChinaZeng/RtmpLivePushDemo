package com.zzw.live.rtmp.encoder;

import android.content.Context;
import android.util.Log;

public class PushEncode extends BasePushEncoder {

    public PushEncode(Context context, int textureId) {
        super(context);
        Log.e("zzz","id = "+textureId);
        setRender(new VideoPushRender(context,textureId));
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

}
