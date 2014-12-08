#include <stdio.h>
#include <stdlib.h>
#include <detect.h> /* for sample_t, event_t, etc. */

typedef struct state_s {
    float * before, * during, * after;
    float b_sum, d_sum, a_sum;
    float last_sample;
    int ix, n;
    float max;
    int max_ix, count;
} state_t;

typedef struct parameters_s {
	float thresh; 
    int rate;
	int width;
} parameters_t;

int detect(event_t * events, sample_t * buf, int nbuf, 
    void * parameters, void * state, sound_setup * setup, 
    struct timeval time)
{
	int k, ixp1, n_events = 0; 
    float power, score, filtered;
    state_t * s = state;
    parameters_t * p = parameters;
    
	// oldest to newest
	for (k = 0; k < nbuf; k++)
	{
        filtered = (float) buf[k] - s->last_sample;
        s->last_sample = buf[k];
        
		power = filtered * filtered;
        ixp1 = (s->ix == (s->n-1)) ? 0 : s->ix+1;
        
        s->before[s->ix] = power; // insert new samples
        s->during[s->ix] = s->before[ixp1];
        s->after[s->ix] = s->during[ixp1];
        
        s->b_sum += (s->before[s->ix] - s->before[ixp1]); // update MSs
        s->d_sum += (s->during[s->ix] - s->during[ixp1]);
        s->a_sum += (s->after[s->ix] - s->after[ixp1]);

        s->ix = ixp1;
        
        score = 10.0*log10f(2.0 * s->d_sum / (s->b_sum + s->a_sum));
        
        if (score > p->thresh)
        {
            if (s->count++ == 0)
            {
                s->max = score; s->max_ix = 0; 
            }
            else if (score > s->max)
            {
                s->max = score; s->max_ix = s->count;
            }
            continue;
        }

        if (s->count == 0)
            continue;
            
        if (s->count > p->width / 4)
        {
            events[n_events].t_in = k - s->max_ix - 2*p->width;
            events[n_events].t_dt = p->width;
            events[n_events].f_lo = 0.0;
            events[n_events].f_df = (float) p->rate / 2.0;
            events[n_events].score = s->max;
            events[n_events].data = NULL; /* no data to return */
            
            n_events++;
        }
        
        s->count = 0;
	}
    
	return n_events;
}

sound_setup * setup(void * parameters)
{
	static sound_setup setup;
    parameters_t * p = parameters;
	
	setup.rate = p->rate;
	setup.N = 8192;
	
	return &setup;
}

void * parameter_initialize(char * config_file) 
{
	static parameters_t parameters;
	parameters.thresh = 3.0;
    parameters.rate = 48000;
	parameters.width = 5000;
    float width;

    FILE * fp = fopen(config_file, "r");
    if (fp)
    {
        int n = fscanf(fp, "%d %f %f", 
            &parameters.rate, &width, &parameters.thresh);
        
        if (n != 3)
            fprintf(stderr, "couldn't read config file\n");
    
        fclose(fp);
        parameters.width = (int) ((float) parameters.rate * width);
    }
   
	return &parameters;
}

void * state_initialize(void * parameters)
{
    parameters_t * p = parameters;
    state_t * s = calloc(1, sizeof(state_t));
    
    s->n = p->width;
    fprintf(stderr,"%d\n", p->width);
    s->before = calloc(s->n, sizeof(float));
    s->during = calloc(s->n, sizeof(float));
    s->after = calloc(s->n, sizeof(float));
    
    s->b_sum = 0; s->d_sum = 0; s->a_sum = 0;
    s->ix = 0;
    s->count = 0;
    
    return s;
}

void cleanup(void * parameters, void * state)
{
    state_t * s = state;
    
    free(s->before);
    free(s->during);
    free(s->after);
    free(s);
    
    /*
     * we don't have to free parameters, because it was declared 
     * static
     */
}



