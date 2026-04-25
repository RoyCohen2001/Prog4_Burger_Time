#pragma once

namespace dae {
	class Commands
	{
	public:
		virtual ~Commands() = default;
		virtual void Execute() = 0;
	};
}