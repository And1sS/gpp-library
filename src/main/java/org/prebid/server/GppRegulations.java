package org.prebid.server;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class GppRegulations {

    static {
        System.loadLibrary("gpplib");
    }

    private Map<String, GppSection> sections = new HashMap<>();

    public GppRegulations() {
    }

    private GppRegulations(Map<String, GppSection> sections) {
        this.sections = Objects.requireNonNull(sections);
    }

    public String encode() {
        return encodeSections(sections);
    }

    public static GppRegulations decode(String encoded) {
        return new GppRegulations(decodeSections(encoded));
    }

    private static native Map<String, GppSection> decodeSections(String encoded);

    private static native String encodeSections(Map<String, GppSection> sections);
}
