#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Common/common.h"

namespace PulseEngine 
{
    /**
     * @brief The Transform class represents a 3D transformation consisting of position, rotation, and scale.
     * It provides all the method relative to the transformation of an entity in the 3D space to easy manipulation.
     * 
     */
    class PULSE_ENGINE_DLL_API Transform 
    {
    public:
        PulseEngine::Vector3 position; // Position in 3D space
        PulseEngine::Vector3 rotation; // Rotation in degrees (pitch, yaw, roll)
        PulseEngine::Vector3 scale;    // Scale factors for each axis

        Transform(const PulseEngine::Vector3& pos = PulseEngine::Vector3(0.0f, 0.0f, 0.0f), 
                  const PulseEngine::Vector3& rot = PulseEngine::Vector3(0.0f, 0.0f, 0.0f), 
                  const PulseEngine::Vector3& scl = PulseEngine::Vector3(1.0f, 1.0f, 1.0f));

        // Transformation operations
        PulseEngine::Vector3 Apply(const PulseEngine::Vector3& point) const;

        /**
         * @brief Get the Forward vector of the transform.
         * 
         * @return PulseEngine::Vector3 
         */
        PulseEngine::Vector3 GetForward() const;
        /**
         * @brief Get the Up vector of the transform.
         * 
         * @return PulseEngine::Vector3 
         */
        PulseEngine::Vector3 GetUp() const;
        /**
         * @brief Get the Right vector of the transform.
         * 
         * @return PulseEngine::Vector3 
         */
        PulseEngine::Vector3 GetRight() const;
    };
}

#endif
