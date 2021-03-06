/*
   Copyright (c) 2014, The Linux Foundation. All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

#define ISMATCH(a,b)    (!strncmp(a,b,PROP_VALUE_MAX))

/* Target-Specific Dalvik Heap Configuration */
void mem_2g() {
        property_set("dalvik.vm.heapstartsize", "16m");
        property_set("dalvik.vm.heapgrowthlimit", "192m");
        property_set("dalvik.vm.heapsize", "512m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "2m");
        property_set("dalvik.vm.heapmaxfree", "8m");
}

void mem_3g() {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "288m");
        property_set("dalvik.vm.heapsize", "768m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "512k");
        property_set("dalvik.vm.heapmaxfree", "8m");
}

void mem_4g() {
	property_set("dalvik.vm.heapgrowthlimit", "384m");
        property_set("dalvik.vm.heapsize", "1024m");
        property_set("dalvik.vm.heaptargetutilization", "0.25");
        property_set("dalvik.vm.heapminfree", "4m");
        property_set("dalvik.vm.heapmaxfree", "16m");
}

void hwui_set() {
        property_set("ro.hwui.texture_cache_size", "72");
        property_set("ro.hwui.layer_cache_size", "48");
        property_set("ro.hwui.r_buffer_cache_size", "8");
        property_set("ro.hwui.path_cache_size", "32");
        property_set("ro.hwui.gradient_cache_size", "1");
        property_set("ro.hwui.drop_shadow_cache_size", "6");
        property_set("ro.hwui.texture_cache_flushrate", "0.4");
        property_set("ro.hwui.text_small_cache_width", "1024");
        property_set("ro.hwui.text_small_cache_height", "1024");
        property_set("ro.hwui.text_large_cache_width", "2048");
        property_set("ro.hwui.text_large_cache_height", "1024");
}

void dualsim() {
        property_set("persist.radio.multisim.config", "dsds");
}

void singlesim() {
        property_set("persist.radio.multisim.config", "");
}

void vendor_load_properties()
{
    char platform[PROP_VALUE_MAX];
    char radio[PROP_VALUE_MAX];
    char sku[PROP_VALUE_MAX];
    char carrier[PROP_VALUE_MAX];
    char device[PROP_VALUE_MAX];
    char devicename[PROP_VALUE_MAX];
    char boot_ram[PROP_VALUE_MAX];
    char multisim[PROP_VALUE_MAX];
    char device_boot[PROP_VALUE_MAX];
    int rc;

    rc = property_get("ro.board.platform", platform);
    if (!rc || !ISMATCH(platform, ANDROID_TARGET))
    return;

    property_get("ro.boot.radio", radio);
    property_get("ro.boot.device", device_boot);
    property_get("ro.boot.hardware.sku", sku);
    property_get("ro.boot.carrier", carrier);

    /* Common for all models */
    property_set("ro.build.product", "athene");
    property_set("ro.product.model", "Moto G (4)");
    property_set("ro.mot.build.customerid", "retail");
    property_set("persist.radio.mot_ecc_custid", "common");
    property_set("persist.radio.mot_ecc_enabled", "1");
    property_set("ro.telephony.default_network", "10,0");
    property_set("ro.hw.device", device_boot);
    property_set("ro.hw.radio", radio);

    /*
     *  Motorola offers single and dual sim in al lot of model numbers
     *  Moved single/dual sim selection outside model list
     */

    /* Force radio to show settings */
    property_set("persist.radio.force_get_pref", "1");
    property_get("ro.boot.dualsim", multisim);
    property_set("ro.hw.dualsim", multisim);

    if ( property_get_bool("ro.boot.dualsim", "false") ) {
       dualsim ();
    } else {
       singlesim ();
    }

    property_get("ro.boot.ram",boot_ram);
    if ( ISMATCH(boot_ram,"4GB")) {
       mem_4g ();
    } else if ( ISMATCH(boot_ram,"3GB")) {
       mem_3g ();
    } else {
       /*
           Even though the 2GB version is most sold, use this
	   as catch-all, just in case ro.boot.ram read fails
           or Motorola decides to introduce another memoty option
        */
       mem_2g ();
    }

    hwui_set ();

    if (ISMATCH(device_boot, "athene_13mp")) {
        /* XT162x  Moto G4 */
        property_set("ro.product.device", "athene");
        property_set("ro.build.description", "athene-user 6.0.1 MPJ24.139-23.4 4 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene/athene:6.0.1/MPJ24.139-23.4/4:user/release-keys");
        property_set("ro.hw.fps", "false");
        property_set("ro.product.model", "Moto G#");
        property_set("ro.telephony.default_network", "10");
    } else {
        /* XT164x Moto G4+ */
        property_set("ro.product.device", "athene_f");
        property_set("ro.build.description", "athene_f-user 6.0.1 MPJ24.139-23.4 4 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene_f/athene_f:6.0.1/MPJ24.139-23.4/4:user/release-keys");
        property_set("ro.hw.fps", "true");
        property_set("ro.product.model", "Moto G# Plus");
        property_set("ro.telephony.default_network", "10,10");
    }

    if (ISMATCH(sku, "XT1625") || ISMATCH(sku, "XT1644")) {
       property_set("persist.radio.is_wps_enabled", "true");
       property_set("ro.radio.imei.sv", "4");
    }

    if (ISMATCH(sku, "XT1621") || ISMATCH(sku, "XT1622") || ISMATCH(sku, "XT1640") || ISMATCH(sku, "XT1642") || ISMATCH(sku, "XT1643")) {
       property_set("ro.radio.imei.sv", "3");
    }

    if (ISMATCH(sku, "XT1626") || ISMATCH(sku, "XT1641")) {
       property_set("ro.radio.imei.sv", "2");
       property_set("persist.radio.is_wps_enabled", "true");
       property_set("persist.radio.pb.max.match", "10");
    }

    property_get("ro.product.device", device);
    strlcpy(devicename, device, sizeof(devicename));
    INFO("Found sku id: %s setting build properties for %s device\n", sku, devicename);
}
