package com.zzw.live.util;

import java.nio.charset.Charset;

/**
 * <h3>协议字节转换常用工具</h3>
 * TODO
 * <h3>Author</h3> （王培学）
 * <h3>Date</h3> 2017/7/18 10:41
 * <h3>Copyright</h3> Copyright (c)2017 Shenzhen Guomaichangxing Technology Co., Ltd. Inc. All rights reserved.
 */
public class ByteUtil {


    /**
     * 从原始字节数组中，从指定位置接受指定长度得到新的字节数组
     *
     * @param original 原始字节数组
     * @param start    开始位置
     * @param length   结束位置
     * @return
     */
    public static byte[] subBytes(byte[] original, int start, int length) {
        byte[] b = new byte[length];
        System.arraycopy(original, start, b, 0, length);
        return b;
    }


    /**
     * 将两个字节数组合并为一个数组
     *
     * @param byte_1
     * @param byte_2
     * @return
     */
    public static byte[] mergerByte(byte[] byte_1, byte[] byte_2) {
        byte[] byte_3 = new byte[byte_1.length + byte_2.length];
        System.arraycopy(byte_1, 0, byte_3, 0, byte_1.length);
        System.arraycopy(byte_2, 0, byte_3, byte_1.length, byte_2.length);
        return byte_3;
    }

    /**
     * 将多个数组合并为一个数据
     *
     * @param bytes
     * @return
     */
    public static byte[] mergerBytes(byte[]... bytes) {
        int len = 0;
        for (byte[] aByte : bytes) {
            len += aByte.length;
        }
        byte[] data = new byte[len];
        int temp = 0;
        for (byte[] aByte : bytes) {
            System.arraycopy(aByte, 0, data, temp, aByte.length);
            temp += aByte.length;
        }
        return data;
    }

    public static byte[] str2Bcd(String asc) {
        int len = asc.length();
        int mod = len % 2;
        if (mod != 0) {
            asc = "0" + asc;
            len = asc.length();
        }
        byte abt[] = new byte[len];
        if (len >= 2) {
            len = len / 2;
        }
        byte bbt[] = new byte[len];
        abt = asc.getBytes();
        int j, k;
        for (int p = 0; p < asc.length() / 2; p++) {
            if ((abt[2 * p] >= '0') && (abt[2 * p] <= '9')) {
                j = abt[2 * p] - '0';
            } else if ((abt[2 * p] >= 'a') && (abt[2 * p] <= 'z')) {
                j = abt[2 * p] - 'a' + 0x0a;
            } else {
                j = abt[2 * p] - 'A' + 0x0a;
            }
            if ((abt[2 * p + 1] >= '0') && (abt[2 * p + 1] <= '9')) {
                k = abt[2 * p + 1] - '0';
            } else if ((abt[2 * p + 1] >= 'a') && (abt[2 * p + 1] <= 'z')) {
                k = abt[2 * p + 1] - 'a' + 0x0a;
            } else {
                k = abt[2 * p + 1] - 'A' + 0x0a;
            }
            int a = (j << 4) + k;
            byte b = (byte) a;
            bbt[p] = b;
            System.out.format("%02X\n", bbt[p]);
        }
        return bbt;
    }

    public static byte asc_to_bcd(byte asc) {
        byte bcd;

        if ((asc >= '0') && (asc <= '9'))
            bcd = (byte) (asc - '0');
        else if ((asc >= 'A') && (asc <= 'F'))
            bcd = (byte) (asc - 'A' + 10);
        else if ((asc >= 'a') && (asc <= 'f'))
            bcd = (byte) (asc - 'a' + 10);
        else
            bcd = (byte) (asc - 48);
        return bcd;
    }

    public static byte[] ASCII_To_BCD(byte[] ascii, int asc_len) {
        byte[] bcd = new byte[asc_len / 2];
        int j = 0;
        for (int i = 0; i < (asc_len + 1) / 2; i++) {
            bcd[i] = asc_to_bcd(ascii[j++]);
            bcd[i] = (byte) (((j >= asc_len) ? 0x00 : asc_to_bcd(ascii[j++])) + (bcd[i] << 4));
            System.out.format("%02X\n", bcd[i]);
        }
        return bcd;
    }

    public static String bytes2Str(byte[] buffer) {
        try {
            boolean flog = false;
            int length = 0;
            for (int i = 0; i < buffer.length; ++i) {
                if (buffer[i] == 0) {
                    flog = true;
                    length = i;
                    break;
                }
            }
            if (length == 0 && !flog) length = buffer.length;

            return new String(buffer, 0, length, "UTF-8");
        } catch (Exception e) {
            return "";
        }
    }

    public static String bcd2Str(byte[] bytes) {
        char temp[] = new char[bytes.length * 2], val;

        for (int i = 0; i < bytes.length; i++) {
            val = (char) (((bytes[i] & 0xf0) >> 4) & 0x0f);
            temp[i * 2] = (char) (val > 9 ? val + 'A' - 10 : val + '0');

            val = (char) (bytes[i] & 0x0f);
            temp[i * 2 + 1] = (char) (val > 9 ? val + 'A' - 10 : val + '0');
        }
        return new String(temp);
    }

    /**
     * 字符串转换为Ascii字符串
     *
     * @param value
     * @return
     */
    public static String stringToAscii(String value) {
        StringBuffer sbu = new StringBuffer();
        char[] chars = value.toCharArray();
        for (int i = 0; i < chars.length; i++) {
            if (i != chars.length - 1) {
                sbu.append((int) chars[i]).append(",");
            } else {
                sbu.append((int) chars[i]);
            }
        }
        return sbu.toString();
    }

    //字符串转换为Ascii字节数组
    public static byte[] stringToAsciiBytes(String value) {
        String[] tempArr = stringToAscii(value).split(",");
        byte[] bytes = new byte[tempArr.length];
        for (int i = 0; i < tempArr.length; i++) {
            bytes[i] = (byte) Integer.parseInt(tempArr[i]);
        }
        return bytes;
    }

    /**
     * Ascii字符串转换为字符串
     *
     * @param value
     * @return
     */
    public static String asciiToString(String value) {
        StringBuffer sbu = new StringBuffer();
        String[] chars = value.split(",");
        for (int i = 0; i < chars.length; i++) {
            sbu.append((char) Integer.parseInt(chars[i]));
        }
        return sbu.toString();
    }

    public static byte[] shortToBytes(short data) {
        byte[] bytes = new byte[2];
        bytes[1] = (byte) (data & 0xff);
        bytes[0] = (byte) ((data & 0xff00) >> 8);
        return bytes;
    }

    public static byte[] charToBytes(char data) {
        byte[] bytes = new byte[2];
        bytes[1] = (byte) (data);
        bytes[0] = (byte) (data >> 8);
        return bytes;
    }

    public static byte[] intToBytes(int data) {
        byte[] bytes = new byte[4];
        bytes[3] = (byte) (data & 0xff);
        bytes[2] = (byte) ((data & 0xff00) >> 8);
        bytes[1] = (byte) ((data & 0xff0000) >> 16);
        bytes[0] = (byte) ((data & 0xff000000) >> 24);
        return bytes;
    }

    public static byte[] longToBytes(long data) {
        byte[] bytes = new byte[8];
        bytes[7] = (byte) (data & 0xff);
        bytes[6] = (byte) ((data >> 8) & 0xff);
        bytes[5] = (byte) ((data >> 16) & 0xff);
        bytes[4] = (byte) ((data >> 24) & 0xff);
        bytes[3] = (byte) ((data >> 32) & 0xff);
        bytes[2] = (byte) ((data >> 40) & 0xff);
        bytes[1] = (byte) ((data >> 48) & 0xff);
        bytes[0] = (byte) ((data >> 56) & 0xff);
        return bytes;
    }

    public static byte[] floatToBytes(float data) {
        int intBits = Float.floatToIntBits(data);
        return intToBytes(intBits);
    }

    public static byte[] doubleToBytes(double data) {
        long intBits = Double.doubleToLongBits(data);
        return longToBytes(intBits);
    }

    public static byte[] stringToBytes(String data, String charsetName) {
        Charset charset = Charset.forName(charsetName);
        return data.getBytes(charset);
    }

    public static byte[] stringToGBKBytes(String data) {
        return stringToBytes(data, "GBK");
    }

    public static byte[] stringToUTF8Bytes(String data) {
        return stringToBytes(data, "UTF-8");
    }


    public static short bytesToShort(byte[] bytes) {
        return (short) ((0xff & bytes[1]) | (0xff00 & (bytes[0] << 8)));
    }

    public static char bytesToChar(byte[] bytes) {
        return (char) ((0xff & bytes[1]) | (0xff00 & (bytes[0] << 8)));
    }

    public static int bytesToInt(byte[] bytes) {
        return (0xff & bytes[3]) | (0xff00 & (bytes[2] << 8)) | (0xff0000 & (bytes[1] << 16))
                | (0xff000000 & (bytes[0] << 24));
    }


    public static long bytesToLong(byte[] bytes) {
        return (0xffL & (long) bytes[7]) | (0xff00L & ((long) bytes[6] << 8)) | (0xff0000L & ((long) bytes[5] << 16))
                | (0xff000000L & ((long) bytes[4] << 24)) | (0xff00000000L & ((long) bytes[3] << 32))
                | (0xff0000000000L & ((long) bytes[2] << 40)) | (0xff000000000000L & ((long) bytes[1] << 48))
                | (0xff00000000000000L & ((long) bytes[0] << 56));
    }

    public static float bytesToFloat(byte[] bytes) {
        return Float.intBitsToFloat(bytesToInt(bytes));
    }

    public static double bytesToDouble(byte[] bytes) {
        long l = bytesToLong(bytes);
        System.out.println(l);
        return Double.longBitsToDouble(l);
    }

    public static String bytesToString(byte[] bytes, String charsetName) {
        return new String(bytes, Charset.forName(charsetName));
    }


    public static String bytesToGBKString(byte[] bytes) {
        return bytesToString(bytes, "GBK");
    }


    public static String bytesToHexString(byte[] src) {
        StringBuilder stringBuilder = new StringBuilder("");
        if (src == null || src.length <= 0) {
            return null;
        }
        for (int i = 0; i < src.length; i++) {
            int v = src[i] & 0xFF;
            String hv = Integer.toHexString(v);
            if (hv.length() < 2) {
                stringBuilder.append(0);
            }
            stringBuilder.append(hv);
        }
        return stringBuilder.toString();
    }

    public static String bytesToHexSpaceString(byte[] src) {
        StringBuilder stringBuilder = new StringBuilder("");
        if (src == null || src.length <= 0) {
            return null;
        }
        for (int i = 0; i < src.length; i++) {
            int v = src[i] & 0xFF;
            String hv = Integer.toHexString(v);
            if (hv.length() < 2) {
                stringBuilder.append(0);
            }
            stringBuilder.append(hv).append(" ");
        }
        return stringBuilder.toString();
    }


    /**
     * 转换成无符号数
     *
     * @param data
     * @return
     */

    public static int getUnsignedByte(byte data) {
        return data & 0x0FF;
    }

    public static int getUnsignedShort(short data) {
        return data & 0x0FFFF;
    }

    public static long getUnsignedInt(int data) {
        return data & 0x0FFFFFFFFl;
    }

    /**
     * 一个2字节数据某一位置1操作
     *
     * @param data
     * @param position
     */
    public static int setBit1(int data, int position) {
        return data = data | (1 << position);
    }

    /**
     * 一个4字节数据某一位置1操作
     *
     * @param data
     * @param position
     */
    public static long setBit1(long data, int position) {
        return data = data | (1 << position);
    }

    /**
     * 给一个2字节数据某一位置0操作
     *
     * @param data
     * @param position
     */
    public static int setBit0(int data, int position) {
        return data = data &= ~(1 << position);
    }

    /**
     * 给一个4字节数据某一位置0操作
     *
     * @param data
     * @param position
     */
    public static long setBit0(long data, int position) {
        return data = data &= ~(1 << position);
    }

    /**
     * 判断某一位是否置位1
     *
     * @param data
     * @param position
     * @return
     */
    public static boolean isBit1(long data, int position) {
        if ((data & (1 << position)) == 1 << position) {
            return true;
        }
        return false;
    }

}
