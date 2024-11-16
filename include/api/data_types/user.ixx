module;

#include <cstdint>
#include <concepts>

export module user;

import tools;

export namespace db_structs {

#pragma pack(push, 1)
	struct UserData {
		uint8_t userId;
		char name[32];
		uint16_t age;
		bool isActive;

		template <tools::OverridableOperator Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar& userId& age& name& isActive;
		}
	};
}