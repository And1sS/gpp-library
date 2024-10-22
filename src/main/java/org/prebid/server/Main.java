package org.prebid.server;

import java.util.Map;

public class Main {

    public static void main(String[] args) {
        System.out.println("Hello world from Java!");
        System.out.println("Decoding result: " + GppRegulations.decode("Test"));
        System.out.println("Encoding result: " + new GppRegulations(Map.of("gpp test key", "gpp val")).encode());
    }
}
