# pragma once

namespace s3d {
	bool isNum(const String str) {
		for (auto& ch : str) {
			if (!IsDigit(ch)) {
				return false;
			}
		}
		return true;
	}
}
