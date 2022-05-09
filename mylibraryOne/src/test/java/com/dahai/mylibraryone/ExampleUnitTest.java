package com.dahai.mylibraryone;

import android.util.Log;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
public class ExampleUnitTest {
    @Test
    public void addition_isCorrect() {
        int value = gPower(3);
        System.out.println("value == " + value);
    }

    int gPower(int n) {
        if (n == 0) {
            return 1;
        }

        int value = gPower(n - 1);
        System.out.println("n == " + n + ";  value == " + value);
        //if (n != 0) {
            value *= 2;
        //}
        // System.out.println("n == " + n + ";  value == " + value);
        return value;
    }
}