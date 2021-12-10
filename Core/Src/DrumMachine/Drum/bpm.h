/*
 * bpm.h
 *
 *  Created on: Nov 17, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_BPM_H_
#define SRC_DRUMMACHINE_BPM_H_

#define MAX_BEAT	175
/* value for MAX_BEAT */
#define	BPM_UNIT	334
/* value for 1 BPM */
#define	BPM_1_BPM	60120
/* Logic : write in counter register the value (BPM_1_BPM - (<bpm> * BPM_UNIT) where <bpm> must be less than MAX_BEAT */
#define	BPM_60_BPM	(60*BPM_UNIT)


#endif /* SRC_APPLICATION_BPM_H_ */
