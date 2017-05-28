#ifndef CONTROL_H
#define CONTROL_H

class trigger {
public:
    trigger(bool &in);
    bool redge();
    bool fedge();

protected:
    bool &d_in;
    bool d_rm, d_fm, d_out;

};

class ton {
public:
    ton(bool &in, const unsigned long pt);
    unsigned long elapsed();
    bool out();
    void rst();

protected:
    trigger d_trig;
    unsigned long d_start, d_end, d_total;
    bool &d_in, d_out, d_init;
    unsigned long d_in_pt, d_pt;
};

class tof {
public:
    tof(bool &in, const unsigned long pt);
    unsigned long elapsed();
    bool out();
    void rst();

protected:
    trigger d_trig;
    unsigned long d_start, d_end;
    bool &d_in, d_out, d_init;
    unsigned long d_in_pt, d_pt;
};

class ctu {
public:
	ctu(bool &in, const unsigned long pv);
	unsigned long cv();
	bool out();
	void rst();

protected:
	trigger d_trig;
	bool &d_in, d_out;
	unsigned long d_in_pv, d_cv;
};

class ctd {
public:
	ctd(bool &in, const unsigned long pv);
	unsigned long cv();
	bool out();
	void rst();

protected:
	trigger d_trig;
	bool &d_in, d_out;
	unsigned long d_in_pv, d_cv;
};

class blink {
public:
	blink(unsigned timelow, unsigned timehigh);
	bool out();
	void set_timelow(const unsigned);
	void set_timehigh(const unsigned);

protected:
	bool d_out;
	unsigned long d_start_off, d_end_off, d_start_on, d_end_on;
	unsigned d_timelow, d_timehigh;
};

#endif
