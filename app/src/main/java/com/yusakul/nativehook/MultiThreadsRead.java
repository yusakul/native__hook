package com.yusakul.nativehook;


import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.Random;

public class MultiThreadsRead extends Thread {

    private byte[] generateBytes() {
        Random random = new Random();
        byte[] bytes = new byte[1024];
        random.nextBytes(bytes);

        return bytes;
    }



    class TestThread implements Runnable {

        @Override
        public void run() {
//			while(lock == 1);
            dowork();
        }

        private synchronized void dowork() {
            Long ll = new Long(Thread.currentThread().getId());
            int threadid = ll.intValue();
            System.out.println("threadid:" + threadid);
            byte[] bytes = generateBytes();
            HookRead.init();
            HookRead.purArray(bytes);
            File file = new File("/sdcard/" + threadid);
            if (!file.exists())
                try {
                    file.createNewFile();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            InputStream is = null;
            try {
                is = new FileInputStream(file);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
            Random random = new Random();
            int j = 0;
            while (true) {
                if (j > 2)
                    break;
                j++;
                int size = random.nextInt(1024);
                byte[] buffer = new byte[size];
                try {
                    System.out.println("read size:" + size);
                    int sz = is.read(buffer);
                    dump(buffer, sz);

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            HookRead.deleteArray();
        }

    }

    private void dump(byte[] bytes, int size) {
        if (bytes == null) {
            System.out.println("bytes is null");
            return;
        }
        System.out.println("bytes length:" + size);
        for (int i = 0; i < size; i++) {
            System.out.print(Byte.toString(bytes[i]) + " ");
        }
        System.out.println();
    }

    @Override
    public void run() {
        super.run();

        Thread thread = new Thread(new TestThread());
        thread.start();
        //
        Thread thread1 = new Thread(new TestThread());
        thread1.start();
        //
        Thread thread2 = new Thread(new TestThread());
        thread2.start();
        //
        Thread thread3 = new Thread(new TestThread());
        thread3.start();
    }
}
