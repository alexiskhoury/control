#include "control.h"
#include <arduino.h>

//trigger class definition
trigger::trigger(bool &in) : d_in(in) {
    d_out = false;
    if (d_in) {
    	d_rm = true; // rising edge memory bit
    	d_fm = false; // falling edge memory bit - reversed because of conflict at object construction
    }
    else {
    	d_rm = false;
    	d_fm = true;
    }
}

bool trigger::redge() {
     d_out = d_in and not d_rm;
     d_rm = d_in;
     return d_out;
}

bool trigger::fedge() {
     d_out = not d_in and not d_fm;
     d_fm = not d_in;
     return d_out;
}

//ton class definition
ton::ton(bool &in, const unsigned long pt) : d_trig(in), d_in(in) {
    d_start = 0;
    d_end = 0;
    d_total = 0;
    d_init = false;
    d_out = false;
    d_in_pt = pt;
    d_pt = 0;
}

unsigned long ton::elapsed() {
    if (d_trig.redge()) {
        d_start = millis();
        d_init = true;
    }
    if (d_trig.fedge()) {
     d_start = d_end = 0; // re-initialize d_start & d_end
     d_pt += d_total; // memorize total period for d_in true state - not iec compliant
     d_total = 0; // re-initialize d_total on falling edge
     d_init = false;
    }
    if (d_init && d_in) {
    d_end = millis();
    d_total = d_end-d_start;
    return d_total+d_pt;
    }
    if (!d_in) {
    d_start = d_end = 0; // re-initialize d_start & d_end
    d_total = 0;
    d_init = false; // should have been set to false by d_trig.fedge()
    return d_pt;
    }
    return 0;
}

bool ton::out() {
    if (this->elapsed() >= d_in_pt) {
        d_out = true;
        d_init = false;
        return d_out;
    } else {
        d_out = false;
        return d_out;
    }
}

void ton::rst() {
    d_start = 0;
    d_end = 0;
    d_total = 0;
    d_init = false;
    d_out = false;
    d_pt = 0;
}

//tof class definition
tof::tof(bool &in, const unsigned long pt) : d_trig(in), d_in(in) {
    d_start = 0;
    d_end = 0;
    d_init = false;
    d_out = false;
    d_in_pt = pt;
    d_pt = 0;
}

unsigned long tof::elapsed() {
    if (d_trig.fedge()) {
        d_start = millis();
        d_init = true; // prevent counting if fedge is not detected
    }
    if (d_trig.redge()) {
     d_start = d_end = 0; // re-initialize d_start & d_end
     d_pt = 0;
     d_init = false;
    }
    if (d_init && !d_in) {
    d_end = millis();
    d_pt = d_end-d_start;
    return d_pt;
    }
    if (d_in) {
    d_start = d_end = 0; // re-initialize d_start & d_end
    d_out = true;
    d_init = false; // d_trig.redge should have set d_init to false
    return d_pt;
    }
    return 0;
}

bool tof::out() {
    if (this->elapsed() >= d_in_pt) {
        d_out = false;
        d_init = false; // stop counting in elapsed function after reaching d_in_pt
        return d_out;
    }
    return d_out;
}

void tof::rst() {
    d_start = 0;
    d_end = 0;
    d_init = false;
    d_out = false;
    d_pt = 0;
}

//ctu class definition
ctu::ctu(bool &in, const unsigned long pv) : d_trig(in), d_in(in) {
	d_in_pv = pv;
	d_out = false;
	d_cv = 0;
}

unsigned long ctu::cv() {
	if (d_trig.redge()) d_cv++;
	if (d_cv > d_in_pv) d_cv = d_in_pv;
	return d_cv;
}

bool ctu::out() {
	this->cv();
	if (d_cv == d_in_pv) {
		d_out = true;
	}
	else {
		d_out = false;
	}
	return d_out;
}

void ctu::rst() {
	d_out = false;
	d_cv = 0;
}

//ctd class definition
ctd::ctd(bool &in, const unsigned long pv) : d_trig(in), d_in(in) {
	d_in_pv = pv;
	d_out = false;
	d_cv = pv;
}

unsigned long ctd::cv() {
	if (d_trig.redge() && d_cv != 0) d_cv--;
	return d_cv;
}

bool ctd::out() {
	this->cv();
	if (d_cv == 0) {
		d_out = true;
	}
	else {
		d_out = false;
	}
	return d_out;
}

void ctd::rst() {
	d_out = false;
	d_cv = d_in_pv;
}
