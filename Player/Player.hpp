#ifndef PLAYER_PLAYER_HPP
#define PLAYER_PLAYER_HPP

#include "../MainInclude.hpp"

#include "../Model/Model.hpp"

namespace players {

	using namespace models;

	class Player {
		private:
			Model* tank;

			// ----- Player Controls ----- //
			bool drivingTank;        // using 3rd person perspective camera
			bool usingBinoculars;   // using 1st person perspective camera
			bool usingOverhead;
			bool resetView;

			// Tank Controls
			bool movingForward;     // W
			bool movingBackward;    // S
			bool turningLeft;       // A
			bool turningRight;      // D

			// Binocular Controls
			bool lookingUp;         // W
			bool lookingDown;       // S
			bool lookingLeft;       // A
			bool lookingRight;      // D
			bool zoomingIn;         // Q
			bool zoomingOut;        // E

			// Drone Controls
			bool droningForward;    // W
			bool droningBackward;   // S
			bool droningRight;      // D
			bool droningLeft;       // A

			/* Lighting */
			// Cycle through intensity of point light using F Key
			bool adjustheadlightsIntensity;
			int headlightsIntensity;
			float lowIntensity;
			float medIntensity;
			float highIntensity;

			// BONUS
			bool jumping;

		public:	
			Player(Model* tank);

		// Getters
		public:
			bool isDrivingTank();
			bool isUsingBinoculars();
			bool isUsingDrone();
			bool isResettingView();
			bool isMovingForward();
			bool isMovingBackward();
			bool isTurningLeft();
			bool isTurningRight();
			bool isLookingUp();
			bool isLookingDown();
			bool isLookingLeft();
			bool isLookingRight();
			bool isZoomingIn();
			bool isZoomingOut();
			bool isDroningForward();
			bool isDroningBackward();
			bool isDroningRight();
			bool isDroningLeft();
			bool isAdjustingHeadlights();

			int getHeadlightsIntensity();
			float getLowIntensity();
			float getMedIntensity();
			float getHighIntensity();

		// Setters
		public:
			void setDrivingTank(bool x);
			void setUsingBinoculars(bool x);
			void setUsingDrone(bool x);
			void setResettingView(bool x);
			void setMovingForward(bool x);
			void setMovingBackward(bool x);
			void setTurningLeft(bool x);
			void setTurningRight(bool x);
			void setLookingUp(bool x);
			void setLookingDown(bool x);
			void setLookingLeft(bool x);
			void setLookingRight(bool x);
			void setZoomingIn(bool x);
			void setZoomingOut(bool x);
			void setDroningForward(bool x);
			void setDroningBackward(bool x);
			void setDroningRight(bool x);
			void setDroningLeft(bool x);
			void setAdjustingHeadlights(bool x);

			void setModel(Model* tank);

		// BONUS: Tank Jump
		public:
			bool isJumping();
			void setJumping(bool x);
	};
}


#endif