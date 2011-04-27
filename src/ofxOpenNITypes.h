#pragma once

// used in ofxTrackedUser
enum ofxTrackedUserOptions {
	 ONI_CONVERT_NONE		=	(1 << 0)
	,ONI_CONVERT_WORLD		=	(1 << 1)
	,ONI_CONVERT_PROJECTIVE =	(1 << 2)
	,ONI_RESCALE_LIMBS		=	(1 << 3) // rescales limb values to cm
};