package org.prebid.server;

public class Main {

    public static void main(String[] args) {
        System.out.println("Hello world from Java!");
        System.out.println("Result of native call: " + GppRegulations.decode("Test STRING!!!!!!!!").encode());
    }
}
