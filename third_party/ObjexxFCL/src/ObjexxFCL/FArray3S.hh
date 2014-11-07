#ifndef ObjexxFCL_FArray3S_hh_INCLUDED
#define ObjexxFCL_FArray3S_hh_INCLUDED

// FArray3S: 3D Slice Array Proxy
//
// Project: Objexx Fortran Compatibility Library (ObjexxFCL)
//
// Version: 4.0.0
//
// Language: C++
//
// Copyright (c) 2000-2014 Objexx Engineering, Inc. All Rights Reserved.
// Use of this source code or any derivative of it is restricted by license.
// Licensing is available from Objexx Engineering, Inc.:  http://objexx.com

// ObjexxFCL Headers
#include <ObjexxFCL/FArray3S.fwd.hh>
#include <ObjexxFCL/FArrayRS.hh>
#include <ObjexxFCL/FArray2S.hh>
#include <ObjexxFCL/MArray3.hh>

namespace ObjexxFCL {

// FArray3S: 3D Slice Array Proxy
template< typename T >
class FArray3S : public FArrayRS< T, 3 >
{

private: // Types

	typedef  FArrayRS< T, 3 >  Super;

private: // Friend

	template< typename > friend class FArray3S;

public: // Types

	typedef  typename Super::Base  Base;
	typedef  typename Super::Traits  Traits;
	typedef  typename Super::IR  IR;
	typedef  typename Super::IS  IS;
	typedef  typename Super::DS  DS;

	// STL Style
	typedef  typename Super::value_type  value_type;
	typedef  typename Super::reference  reference;
	typedef  typename Super::const_reference  const_reference;
	typedef  typename Super::pointer  pointer;
	typedef  typename Super::const_pointer  const_pointer;
	typedef  typename Super::size_type  size_type;
	typedef  typename Super::difference_type  difference_type;

	// C++ Style
	typedef  typename Super::Value  Value;
	typedef  typename Super::Reference  Reference;
	typedef  typename Super::ConstReference  ConstReference;
	typedef  typename Super::Pointer  Pointer;
	typedef  typename Super::ConstPointer  ConstPointer;
	typedef  typename Super::Size  Size;
	typedef  typename Super::Difference  Difference;

	// Using
	using Super::in_range;
	using Super::isize;
	using Super::overlap;
	using Super::size;
	using Super::slice_k;
	using Super::data_;
	using Super::data_beg_;
	using Super::data_end_;
	using Super::size_;

public: // Creation

	// Default Constructor
	inline
	FArray3S() :
	 m1_( 1 ),
	 m2_( 1 ),
	 m3_( 1 ),
	 k_( 0 ),
	 u1_( 0 ),
	 u2_( 0 ),
	 u3_( 0 )
	{}

	// Copy Constructor
	inline
	FArray3S( FArray3S const & a ) :
	 Super( a ),
	 m1_( a.m1_ ),
	 m2_( a.m2_ ),
	 m3_( a.m3_ ),
	 k_( a.k_ ),
	 u1_( a.u1_ ),
	 u2_( a.u2_ ),
	 u3_( a.u3_ )
	{
		data_set();
	}

	// Data Constructor
	inline
	FArray3S( T const * data, std::int64_t const k, DS const & d1, DS const & d2, DS const & d3 ) :
	 Super( data, d1.z() * d2.z() * d3.z() ),
	 m1_( d1.m() ),
	 m2_( d2.m() ),
	 m3_( d3.m() ),
	 k_( k + d1.k() + d2.k() + d3.k() ),
	 u1_( d1.u() ),
	 u2_( d2.u() ),
	 u3_( d3.u() )
	{
		data_set();
	}

	// Non-Const Data Constructor
	inline
	FArray3S( T * data, std::int64_t const k, DS const & d1, DS const & d2, DS const & d3 ) :
	 Super( data, d1.z() * d2.z() * d3.z() ),
	 m1_( d1.m() ),
	 m2_( d2.m() ),
	 m3_( d3.m() ),
	 k_( k + d1.k() + d2.k() + d3.k() ),
	 u1_( d1.u() ),
	 u2_( d2.u() ),
	 u3_( d3.u() )
	{
		data_set();
	}

	// Array Constructor
	template< template< typename > class Array >
	inline
	FArray3S( Array< T > const & a ) :
	 Super( a.data(), a.size() ),
	 m1_( 1 ),
	 m2_( a.size1() ),
	 m3_( a.size2() * m2_ ),
	 k_( -( m1_ + m2_ + m3_ ) ),
	 u1_( a.isize1() ),
	 u2_( a.isize2() ),
	 u3_( a.isize3() )
	{
		data_set();
	}

	// Destructor
	inline
	virtual
	~FArray3S()
	{}

public: // Assignment: Array

	// Copy Assignment
	inline
	FArray3S &
	operator =( FArray3S const & a )
	{
		if ( this != &a ) {
			assert( conformable( a ) );
			if ( overlap( a ) ) { // Overlap-safe
				CArray< T > c( size_ );
				size_type l( 0 );
				for ( int i3 = 1; i3 <= u3_; ++i3 ) {
					for ( int i2 = 1; i2 <= u2_; ++i2 ) {
						for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
							c[ l ] = a( i1, i2, i3 );
						}
					}
				}
				l = 0;
				for ( int i3 = 1; i3 <= u3_; ++i3 ) {
					for ( int i2 = 1; i2 <= u2_; ++i2 ) {
						for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
							operator ()( i1, i2, i3 ) = c[ l ];
						}
					}
				}
			} else { // Not overlap-safe
				for ( int i3 = 1; i3 <= u3_; ++i3 ) {
					for ( int i2 = 1; i2 <= u2_; ++i2 ) {
						for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
							operator ()( i1, i2, i3 ) = a( i1, i2, i3 );
						}
					}
				}
			}
		}
		return *this;
	}

	// Copy Assignment Template
	template< typename U, class = typename std::enable_if< std::is_assignable< T&, U >::value >::type >
	inline
	FArray3S &
	operator =( FArray3S< U > const & a )
	{
		assert( conformable( a ) );
		for ( int i3 = 1; i3 <= u3_; ++i3 ) {
			for ( int i2 = 1; i2 <= u2_; ++i2 ) {
				for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
					operator ()( i1, i2, i3 ) = a( i1, i2, i3 );
				}
			}
		}
		return *this;
	}

	// MArray Assignment Template
	template< class A, typename M >
	inline
	FArray3S &
	operator =( MArray3< A, M > const & a )
	{
		assert( conformable( a ) );
		if ( a.dimensions_initialized() ) {
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
						operator ()( i1, i2, i3 ) = a( i1, i2, i3 );
					}
				}
			}
		}
		return *this;
	}

	// Array Assignment Template
	template< template< typename > class ArrayType >
	inline
	FArray3S &
	operator =( ArrayType< T > const & a )
	{
		assert( conformable( a ) );
		if ( overlap( a ) ) { // Overlap-safe
			CArray< T > c( size_ );
			size_type l( 0 );
			for ( int j3 = a.l3(), e3 = a.u3(); j3 <= e3; ++j3 ) {
				for ( int j2 = a.l2(), e2 = a.u2(); j2 <= e2; ++j2 ) {
					for ( int j1 = a.l1(), e1 = a.u1(); j1 <= e1; ++j1, ++l ) {
						c[ l ] = a( j1, j2, j3 );
					}
				}
			}
			l = 0;
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
						operator ()( i1, i2, i3 ) = c[ l ];
					}
				}
			}
		} else { // Not overlap-safe
			for ( int i3 = 1, j3 = a.l3(); i3 <= u3_; ++i3, ++j3 ) {
				for ( int i2 = 1, j2 = a.l2(); i2 <= u2_; ++i2, ++j2 ) {
					for ( int i1 = 1, j1 = a.l1(), e1 = u1_; i1 <= e1; ++i1, ++j1 ) {
						operator ()( i1, i2, i3 ) = a( j1, j2, j3 );
					}
				}
			}
		}
		return *this;
	}

	// Array Assignment Template
	template< template< typename > class ArrayType, typename U, class = typename std::enable_if< std::is_assignable< T&, U >::value >::type >
	inline
	FArray3S &
	operator =( ArrayType< U > const & a )
	{
		assert( conformable( a ) );
		for ( int i3 = 1, j3 = a.l3(); i3 <= u3_; ++i3, ++j3 ) {
			for ( int i2 = 1, j2 = a.l2(); i2 <= u2_; ++i2, ++j2 ) {
				for ( int i1 = 1, j1 = a.l1(), e1 = u1_; i1 <= e1; ++i1, ++j1 ) {
					operator ()( i1, i2, i3 ) = a( j1, j2, j3 );
				}
			}
		}
		return *this;
	}

	// Initializer List Assignment Template
	template< typename U, class = typename std::enable_if< std::is_assignable< T&, U >::value >::type >
	inline
	FArray3S &
	operator =( std::initializer_list< U > const l )
	{
		assert( size_ == l.size() );
		auto r( l.begin() );
		for ( int i3 = 1; i3 <= u3_; ++i3 ) {
			for ( int i2 = 1; i2 <= u2_; ++i2 ) {
				for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++r ) {
					operator ()( i1, i2, i3 ) = *r;
				}
			}
		}
		return *this;
	}

	// += MArray Template
	template< class A, typename M >
	inline
	FArray3S &
	operator +=( MArray3< A, M > const & a )
	{
		assert( conformable( a ) );
		if ( a.dimensions_initialized() ) {
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
						operator () ( i1, i2, i3 ) += a( i1, i2, i3 );
					}
				}
			}
		}
		return *this;
	}

	// -= MArray Template
	template< class A, typename M >
	inline
	FArray3S &
	operator -=( MArray3< A, M > const & a )
	{
		assert( conformable( a ) );
		if ( a.dimensions_initialized() ) {
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
						operator () ( i1, i2, i3 ) -= a( i1, i2, i3 );
					}
				}
			}
		}
		return *this;
	}

	// *= MArray Template
	template< class A, typename M >
	inline
	FArray3S &
	operator *=( MArray3< A, M > const & a )
	{
		assert( conformable( a ) );
		if ( a.dimensions_initialized() ) {
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
						operator () ( i1, i2, i3 ) *= a( i1, i2, i3 );
					}
				}
			}
		}
		return *this;
	}

	// /= MArray Template
	template< class A, typename M >
	inline
	FArray3S &
	operator /=( MArray3< A, M > const & a )
	{
		assert( conformable( a ) );
		if ( a.dimensions_initialized() ) {
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
						assert( T( a( i1, i2, i3 ) ) != T( 0 ) );
						operator () ( i1, i2, i3 ) /= a( i1, i2, i3 );
					}
				}
			}
		}
		return *this;
	}

	// += Array Template
	template< template< typename > class ArrayType >
	inline
	FArray3S &
	operator +=( ArrayType< T > const & a )
	{
		assert( conformable( a ) );
		if ( overlap( a ) ) { // Overlap-safe
			CArray< T > c( size_ );
			size_type l( 0 );
			for ( int j3 = a.l3(), e3 = a.u3(); j3 <= e3; ++j3 ) {
				for ( int j2 = a.l2(), e2 = a.u2(); j2 <= e2; ++j2 ) {
					for ( int j1 = a.l1(), e1 = a.u1(); j1 <= e1; ++j1, ++l ) {
						c[ l ] = a( j1, j2, j3 );
					}
				}
			}
			l = 0;
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
						operator ()( i1, i2, i3 ) += c[ l ];
					}
				}
			}
		} else { // Not overlap-safe
			for ( int i3 = 1, j3 = a.l3(); i3 <= u3_; ++i3, ++j3 ) {
				for ( int i2 = 1, j2 = a.l2(); i2 <= u2_; ++i2, ++j2 ) {
					for ( int i1 = 1, j1 = a.l1(), e1 = u1_; i1 <= e1; ++i1, ++j1 ) {
						operator ()( i1, i2, i3 ) += a( j1, j2, j3 );
					}
				}
			}
		}
		return *this;
	}

	// -= Array Template
	template< template< typename > class ArrayType >
	inline
	FArray3S &
	operator -=( ArrayType< T > const & a )
	{
		assert( conformable( a ) );
		if ( overlap( a ) ) { // Overlap-safe
			CArray< T > c( size_ );
			size_type l( 0 );
			for ( int j3 = a.l3(), e3 = a.u3(); j3 <= e3; ++j3 ) {
				for ( int j2 = a.l2(), e2 = a.u2(); j2 <= e2; ++j2 ) {
					for ( int j1 = a.l1(), e1 = a.u1(); j1 <= e1; ++j1, ++l ) {
						c[ l ] = a( j1, j2, j3 );
					}
				}
			}
			l = 0;
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
						operator ()( i1, i2, i3 ) -= c[ l ];
					}
				}
			}
		} else { // Not overlap-safe
			for ( int i3 = 1, j3 = a.l3(); i3 <= u3_; ++i3, ++j3 ) {
				for ( int i2 = 1, j2 = a.l2(); i2 <= u2_; ++i2, ++j2 ) {
					for ( int i1 = 1, j1 = a.l1(), e1 = u1_; i1 <= e1; ++i1, ++j1 ) {
						operator ()( i1, i2, i3 ) -= a( j1, j2, j3 );
					}
				}
			}
		}
		return *this;
	}

	// *= Array Template
	template< template< typename > class ArrayType >
	inline
	FArray3S &
	operator *=( ArrayType< T > const & a )
	{
		assert( conformable( a ) );
		if ( overlap( a ) ) { // Overlap-safe
			CArray< T > c( size_ );
			size_type l( 0 );
			for ( int j3 = a.l3(), e3 = a.u3(); j3 <= e3; ++j3 ) {
				for ( int j2 = a.l2(), e2 = a.u2(); j2 <= e2; ++j2 ) {
					for ( int j1 = a.l1(), e1 = a.u1(); j1 <= e1; ++j1, ++l ) {
						c[ l ] = a( j1, j2, j3 );
					}
				}
			}
			l = 0;
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
						operator ()( i1, i2, i3 ) *= c[ l ];
					}
				}
			}
		} else { // Not overlap-safe
			for ( int i3 = 1, j3 = a.l3(); i3 <= u3_; ++i3, ++j3 ) {
				for ( int i2 = 1, j2 = a.l2(); i2 <= u2_; ++i2, ++j2 ) {
					for ( int i1 = 1, j1 = a.l1(), e1 = u1_; i1 <= e1; ++i1, ++j1 ) {
						operator ()( i1, i2, i3 ) *= a( j1, j2, j3 );
					}
				}
			}
		}
		return *this;
	}

	// /= Array Template
	template< template< typename > class ArrayType >
	inline
	FArray3S &
	operator /=( ArrayType< T > const & a )
	{
		assert( conformable( a ) );
		if ( overlap( a ) ) { // Overlap-safe
			CArray< T > c( size_ );
			size_type l( 0 );
			for ( int j3 = a.l3(), e3 = a.u3(); j3 <= e3; ++j3 ) {
				for ( int j2 = a.l2(), e2 = a.u2(); j2 <= e2; ++j2 ) {
					for ( int j1 = a.l1(), e1 = a.u1(); j1 <= e1; ++j1, ++l ) {
						assert( T( a( j1, j2, j3 ) ) != T( 0 ) );
						c[ l ] = a( j1, j2, j3 );
					}
				}
			}
			l = 0;
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
						operator ()( i1, i2, i3 ) /= c[ l ];
					}
				}
			}
		} else { // Not overlap-safe
			for ( int i3 = 1, j3 = a.l3(); i3 <= u3_; ++i3, ++j3 ) {
				for ( int i2 = 1, j2 = a.l2(); i2 <= u2_; ++i2, ++j2 ) {
					for ( int i1 = 1, j1 = a.l1(), e1 = u1_; i1 <= e1; ++i1, ++j1 ) {
						assert( T( a( j1, j2, j3 ) ) != T( 0 ) );
						operator ()( i1, i2, i3 ) /= a( j1, j2, j3 );
					}
				}
			}
		}
		return *this;
	}

	// += Array Template
	template< template< typename > class ArrayType, typename U, class = typename std::enable_if< std::is_assignable< T&, U >::value >::type >
	inline
	FArray3S &
	operator +=( ArrayType< U > const & a )
	{
		assert( conformable( a ) );
		for ( int i3 = 1, j3 = a.l3(); i3 <= u3_; ++i3, ++j3 ) {
			for ( int i2 = 1, j2 = a.l2(); i2 <= u2_; ++i2, ++j2 ) {
				for ( int i1 = 1, j1 = a.l1(), e1 = u1_; i1 <= e1; ++i1, ++j1 ) {
					operator ()( i1, i2, i3 ) += a( j1, j2, j3 );
				}
			}
		}
		return *this;
	}

	// -= Array Template
	template< template< typename > class ArrayType, typename U, class = typename std::enable_if< std::is_assignable< T&, U >::value >::type >
	inline
	FArray3S &
	operator -=( ArrayType< U > const & a )
	{
		assert( conformable( a ) );
		for ( int i3 = 1, j3 = a.l3(); i3 <= u3_; ++i3, ++j3 ) {
			for ( int i2 = 1, j2 = a.l2(); i2 <= u2_; ++i2, ++j2 ) {
				for ( int i1 = 1, j1 = a.l1(), e1 = u1_; i1 <= e1; ++i1, ++j1 ) {
					operator ()( i1, i2, i3 ) -= a( j1, j2, j3 );
				}
			}
		}
		return *this;
	}

	// *= Array Template
	template< template< typename > class ArrayType, typename U, class = typename std::enable_if< std::is_assignable< T&, U >::value >::type >
	inline
	FArray3S &
	operator *=( ArrayType< U > const & a )
	{
		assert( conformable( a ) );
		for ( int i3 = 1, j3 = a.l3(); i3 <= u3_; ++i3, ++j3 ) {
			for ( int i2 = 1, j2 = a.l2(); i2 <= u2_; ++i2, ++j2 ) {
				for ( int i1 = 1, j1 = a.l1(), e1 = u1_; i1 <= e1; ++i1, ++j1 ) {
					operator ()( i1, i2, i3 ) *= a( j1, j2, j3 );
				}
			}
		}
		return *this;
	}

	// /= Array Template
	template< template< typename > class ArrayType, typename U, class = typename std::enable_if< std::is_assignable< T&, U >::value >::type >
	inline
	FArray3S &
	operator /=( ArrayType< U > const & a )
	{
		assert( conformable( a ) );
		for ( int i3 = 1, j3 = a.l3(); i3 <= u3_; ++i3, ++j3 ) {
			for ( int i2 = 1, j2 = a.l2(); i2 <= u2_; ++i2, ++j2 ) {
				for ( int i1 = 1, j1 = a.l1(), e1 = u1_; i1 <= e1; ++i1, ++j1 ) {
					assert( T( a( j1, j2, j3 ) ) != T( 0 ) );
					operator ()( i1, i2, i3 ) /= a( j1, j2, j3 );
				}
			}
		}
		return *this;
	}

public: // Assignment: Logical

	// &&= Array
	inline
	FArray3S &
	and_equals( FArray3S const & a )
	{
		assert( conformable( a ) );
		if ( overlap( a ) ) { // Overlap-safe
			CArray< T > c( size_ );
			size_type l( 0 );
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
						c[ l ] = a( i1, i2, i3 );
					}
				}
			}
			l = 0;
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
						auto & v( operator ()( i1, i2, i3 ) );
						v = v && c[ l ];
					}
				}
			}
		} else { // Not overlap-safe
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
						auto & v( operator ()( i1, i2, i3 ) );
						v = v && a( i1, i2, i3 );
					}
				}
			}
		}
		return *this;
	}

	// ||= Array
	inline
	FArray3S &
	or_equals( FArray3S const & a )
	{
		assert( conformable( a ) );
		if ( overlap( a ) ) { // Overlap-safe
			CArray< T > c( size_ );
			size_type l( 0 );
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
						c[ l ] = a( i1, i2, i3 );
					}
				}
			}
			l = 0;
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1, ++l ) {
						auto & v( operator ()( i1, i2, i3 ) );
						v = v || c[ l ];
					}
				}
			}
		} else { // Not overlap-safe
			for ( int i3 = 1; i3 <= u3_; ++i3 ) {
				for ( int i2 = 1; i2 <= u2_; ++i2 ) {
					for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
						auto & v( operator ()( i1, i2, i3 ) );
						v = v || a( i1, i2, i3 );
					}
				}
			}
		}
		return *this;
	}

	// &&= Array Template
	template< typename U, class = typename std::enable_if< std::is_assignable< T&, U >::value >::type >
	inline
	FArray3S &
	and_equals( FArray3S const & a )
	{
		assert( conformable( a ) );
		for ( int i3 = 1; i3 <= u3_; ++i3 ) {
			for ( int i2 = 1; i2 <= u2_; ++i2 ) {
				for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
					auto & v( operator ()( i1, i2, i3 ) );
					v = v && a( i1, i2, i3 );
				}
			}
		}
		return *this;
	}

	// ||= Array Template
	template< typename U, class = typename std::enable_if< std::is_assignable< T&, U >::value >::type >
	inline
	FArray3S &
	or_equals( FArray3S const & a )
	{
		assert( conformable( a ) );
		for ( int i3 = 1; i3 <= u3_; ++i3 ) {
			for ( int i2 = 1; i2 <= u2_; ++i2 ) {
				for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
					auto & v( operator ()( i1, i2, i3 ) );
					v = v || a( i1, i2, i3 );
				}
			}
		}
		return *this;
	}

public: // Assignment: Value

	// = Value
	inline
	FArray3S &
	operator =( T const & t )
	{
		for ( int i3 = 1; i3 <= u3_; ++i3 ) {
			for ( int i2 = 1; i2 <= u2_; ++i2 ) {
				for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
					operator ()( i1, i2, i3 ) = t;
				}
			}
		}
		return *this;
	}

	// += Value
	inline
	FArray3S &
	operator +=( T const & t )
	{
		for ( int i3 = 1; i3 <= u3_; ++i3 ) {
			for ( int i2 = 1; i2 <= u2_; ++i2 ) {
				for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
					operator ()( i1, i2, i3 ) += t;
				}
			}
		}
		return *this;
	}

	// -= Value
	inline
	FArray3S &
	operator -=( T const & t )
	{
		for ( int i3 = 1; i3 <= u3_; ++i3 ) {
			for ( int i2 = 1; i2 <= u2_; ++i2 ) {
				for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
					operator ()( i1, i2, i3 ) -= t;
				}
			}
		}
		return *this;
	}

	// *= Value
	inline
	FArray3S &
	operator *=( T const & t )
	{
		for ( int i3 = 1; i3 <= u3_; ++i3 ) {
			for ( int i2 = 1; i2 <= u2_; ++i2 ) {
				for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
					operator ()( i1, i2, i3 ) *= t;
				}
			}
		}
		return *this;
	}

	// /= Value
	inline
	FArray3S &
	operator /=( T const & t )
	{
		assert( t != T( 0 ) );
		for ( int i3 = 1; i3 <= u3_; ++i3 ) {
			for ( int i2 = 1; i2 <= u2_; ++i2 ) {
				for ( int i1 = 1, e1 = u1_; i1 <= e1; ++i1 ) {
					operator ()( i1, i2, i3 ) /= t;
				}
			}
		}
		return *this;
	}

public: // Subscript

	// array( i1, i2, i3 ) const
	inline
	T const &
	operator ()( int const i1, int const i2, int const i3 ) const
	{
		assert( contains( i1, i2, i3 ) );
		return data_[ k_ + ( m1_ * i1 ) + ( m2_ * i2 ) + ( m3_ * i3 ) ];
	}

	// array( i1, i2, i3 )
	inline
	T &
	operator ()( int const i1, int const i2, int const i3 )
	{
		assert( contains( i1, i2, i3 ) );
		return data_[ k_ + ( m1_ * i1 ) + ( m2_ * i2 ) + ( m3_ * i3 ) ];
	}

	// Linear Index
	inline
	size_type
	index( int const i1, int const i2, int const i3 ) const
	{
		return k_ + ( m1_ * i1 ) + ( m2_ * i2 ) + ( m3_ * i3 );
	}

public: // Slice Proxy Generators

	// array( s1, s2, s3 ) const
	inline
	FArray3S< T >
	operator ()( IS const & s1, IS const & s2, IS const & s3 ) const
	{
		DS const d1( u1_, s1, m1_ );
		DS const d2( u2_, s2, m2_ );
		DS const d3( u3_, s3, m3_ );
		return FArray3S( data_, k_, d1, d2, d3 );
	}

	// array( i1, s2, s3 ) const
	inline
	FArray2S< T >
	operator ()( int const i1, IS const & s2, IS const & s3 ) const
	{
		std::int64_t k( k_ );
		k += slice_k( u1_, i1, m1_ );
		DS const d2( u2_, s2, m2_ );
		DS const d3( u3_, s3, m3_ );
		return FArray2S< T >( data_, k, d2, d3 );
	}

	// array( s1, i2, s3 ) const
	inline
	FArray2S< T >
	operator ()( IS const & s1, int const i2, IS const & s3 ) const
	{
		std::int64_t k( k_ );
		DS const d1( u1_, s1, m1_ );
		k += slice_k( u2_, i2, m2_ );
		DS const d3( u3_, s3, m3_ );
		return FArray2S< T >( data_, k, d1, d3 );
	}

	// array( s1, s2, i3 ) const
	inline
	FArray2S< T >
	operator ()( IS const & s1, IS const & s2, int const i3 ) const
	{
		std::int64_t k( k_ );
		DS const d1( u1_, s1, m1_ );
		DS const d2( u2_, s2, m2_ );
		k += slice_k( u3_, i3, m3_ );
		return FArray2S< T >( data_, k, d1, d2 );
	}

	// array( s1, i2, i3 ) const
	inline
	FArray1S< T >
	operator ()( IS const & s1, int const i2, int const i3 ) const
	{
		std::int64_t k( k_ );
		DS const d1( u1_, s1, m1_ );
		k += slice_k( u2_, i2, m2_ );
		k += slice_k( u3_, i3, m3_ );
		return FArray1S< T >( data_, k, d1 );
	}

	// array( i1, s2, i3 ) const
	inline
	FArray1S< T >
	operator ()( int const i1, IS const & s2, int const i3 ) const
	{
		std::int64_t k( k_ );
		k += slice_k( u1_, i1, m1_ );
		DS const d2( u2_, s2, m2_ );
		k += slice_k( u3_, i3, m3_ );
		return FArray1S< T >( data_, k, d2 );
	}

	// array( i1, i2, s3 ) const
	inline
	FArray1S< T >
	operator ()( int const i1, int const i2, IS const & s3 ) const
	{
		std::int64_t k( k_ );
		k += slice_k( u1_, i1, m1_ );
		k += slice_k( u2_, i2, m2_ );
		DS const d3( u3_, s3, m3_ );
		return FArray1S< T >( data_, k, d3 );
	}

	// array( s1, s2, s3 )
	inline
	FArray3S< T >
	operator ()( IS const & s1, IS const & s2, IS const & s3 )
	{
		DS const d1( u1_, s1, m1_ );
		DS const d2( u2_, s2, m2_ );
		DS const d3( u3_, s3, m3_ );
		return FArray3S( data_, k_, d1, d2, d3 );
	}

	// array( i1, s2, s3 )
	inline
	FArray2S< T >
	operator ()( int const i1, IS const & s2, IS const & s3 )
	{
		std::int64_t k( k_ );
		k += slice_k( u1_, i1, m1_ );
		DS const d2( u2_, s2, m2_ );
		DS const d3( u3_, s3, m3_ );
		return FArray2S< T >( data_, k, d2, d3 );
	}

	// array( s1, i2, s3 )
	inline
	FArray2S< T >
	operator ()( IS const & s1, int const i2, IS const & s3 )
	{
		std::int64_t k( k_ );
		DS const d1( u1_, s1, m1_ );
		k += slice_k( u2_, i2, m2_ );
		DS const d3( u3_, s3, m3_ );
		return FArray2S< T >( data_, k, d1, d3 );
	}

	// array( s1, s2, i3 )
	inline
	FArray2S< T >
	operator ()( IS const & s1, IS const & s2, int const i3 )
	{
		std::int64_t k( k_ );
		DS const d1( u1_, s1, m1_ );
		DS const d2( u2_, s2, m2_ );
		k += slice_k( u3_, i3, m3_ );
		return FArray2S< T >( data_, k, d1, d2 );
	}

	// array( s1, i2, i3 )
	inline
	FArray1S< T >
	operator ()( IS const & s1, int const i2, int const i3 )
	{
		std::int64_t k( k_ );
		DS const d1( u1_, s1, m1_ );
		k += slice_k( u2_, i2, m2_ );
		k += slice_k( u3_, i3, m3_ );
		return FArray1S< T >( data_, k, d1 );
	}

	// array( i1, s2, i3 )
	inline
	FArray1S< T >
	operator ()( int const i1, IS const & s2, int const i3 )
	{
		std::int64_t k( k_ );
		k += slice_k( u1_, i1, m1_ );
		DS const d2( u2_, s2, m2_ );
		k += slice_k( u3_, i3, m3_ );
		return FArray1S< T >( data_, k, d2 );
	}

	// array( i1, i2, s3 )
	inline
	FArray1S< T >
	operator ()( int const i1, int const i2, IS const & s3 )
	{
		std::int64_t k( k_ );
		k += slice_k( u1_, i1, m1_ );
		k += slice_k( u2_, i2, m2_ );
		DS const d3( u3_, s3, m3_ );
		return FArray1S< T >( data_, k, d3 );
	}

public: // Predicate

	// contains( i1, i2, i3 )
	inline
	bool
	contains( int const i1, int const i2, int const i3 ) const
	{
		if ( ! in_range( u1(), i1 ) ) return false;
		if ( ! in_range( u2(), i2 ) ) return false;
		if ( ! in_range( u3(), i3 ) ) return false;
		return true;
	}

	// Conformable?
	template< typename U >
	inline
	bool
	conformable( FArray3S< U > const & a ) const
	{
		return ( ( size1() == a.size1() ) && ( size2() == a.size2() ) && ( size3() == a.size3() ) );
	}

	// Conformable?
	template< class A, typename M >
	inline
	bool
	conformable( MArray3< A, M > const & a ) const
	{
		return ( ( size1() == a.size1() ) && ( size2() == a.size2() ) && ( size3() == a.size3() ) );
	}

	// Conformable?
	template< class ArrayType >
	inline
	bool
	conformable( ArrayType const & a ) const
	{
		return ( ( a.rank() == 3 ) && ( size1() == a.size1() ) && ( size2() == a.size2() ) && ( size3() == a.size3() ) );
	}

	// Equal Dimensions?
	template< typename U >
	inline
	bool
	equal_dimensions( FArray3S< U > const & a ) const
	{
		return conformable( a );
	}

	// Equal Dimensions?
	template< class A, typename M >
	inline
	bool
	equal_dimensions( MArray3< A, M > const & a ) const
	{
		return conformable( a );
	}

	// Equal Dimensions?
	template< class ArrayType >
	inline
	bool
	equal_dimensions( ArrayType const & a ) const
	{
		return conformable( a );
	}

public: // Inspector

	// IndexRange of a Dimension
	inline
	IR
	I( int const d ) const
	{
		switch ( d ) {
		case 1:
			return I1();
		case 2:
			return I2();
		case 3:
			return I3();
		default:
			assert( false );
			return I1();
		}
	}

	// Upper Index of a Dimension
	inline
	int
	u( int const d ) const
	{
		switch ( d ) {
		case 1:
			return u1_;
		case 2:
			return u2_;
		case 3:
			return u3_;
		default:
			assert( false );
			return u1_;
		}
	}

	// Size of a Dimension
	inline
	size_type
	size( int const d ) const
	{
		switch ( d ) {
		case 1:
			return size1();
		case 2:
			return size2();
		case 3:
			return size3();
		default:
			assert( false );
			return size1();
		}
	}

	// Size of a Dimension
	inline
	int
	isize( int const d ) const
	{
		switch ( d ) {
		case 1:
			return isize1();
		case 2:
			return isize2();
		case 3:
			return isize3();
		default:
			assert( false );
			return isize1();
		}
	}

	// IndexRange of Dimension 1
	inline
	IR
	I1() const
	{
		return IR( 1, u1_ );
	}

	// Lower Index of Dimension 1
	inline
	int
	l1() const
	{
		return 1;
	}

	// Upper Index of Dimension 1
	inline
	int
	u1() const
	{
		return u1_;
	}

	// Size of Dimension 1
	inline
	size_type
	size1() const
	{
		return u1_;
	}

	// Size of Dimension 1
	inline
	int
	isize1() const
	{
		return u1_;
	}

	// IndexRange of Dimension 2
	inline
	IR
	I2() const
	{
		return IR( 1, u2_ );
	}

	// Lower Index of Dimension 2
	inline
	int
	l2() const
	{
		return 1;
	}

	// Upper Index of Dimension 2
	inline
	int
	u2() const
	{
		return u2_;
	}

	// Size of Dimension 2
	inline
	size_type
	size2() const
	{
		return u2_;
	}

	// Size of Dimension 2
	inline
	int
	isize2() const
	{
		return u2_;
	}

	// IndexRange of Dimension 3
	inline
	IR
	I3() const
	{
		return IR( 1, u3_ );
	}

	// Lower Index of Dimension 3
	inline
	int
	l3() const
	{
		return 1;
	}

	// Upper Index of Dimension 3
	inline
	int
	u3() const
	{
		return u3_;
	}

	// Size of Dimension 3
	inline
	size_type
	size3() const
	{
		return u3_;
	}

	// Size of Dimension 3
	inline
	int
	isize3() const
	{
		return u3_;
	}

public: // MArray Generators

	// Template Helpers
	template< typename U > class Wrapper {};
	typedef  typename std::conditional< std::is_class< T >::value, T, Wrapper< T > >::type  ClassT;

	// MArray Generator
	template< typename M >
	inline
	MArray3< FArray3S const, M >
	ma( M ClassT::* pmem ) const
	{
		return MArray3< FArray3S const, M >( *this, pmem );
	}

	// MArray Generator
	template< typename M >
	inline
	MArray3< FArray3S, M >
	ma( M ClassT::* pmem )
	{
		return MArray3< FArray3S, M >( *this, pmem );
	}

public: // Comparison: Predicate

	// Slice == Slice
	inline
	friend
	bool
	eq( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( ( &a == &b ) || a.empty() ) return true;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) == b( i1, i2, i3 ) ) ) return false;
				}
			}
		}
		return true;
	}

	// Slice != Slice
	inline
	friend
	bool
	ne( FArray3S const & a, FArray3S const & b )
	{
		return ! eq( a, b );
	}

	// Slice < Slice
	inline
	friend
	bool
	lt( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( ( &a == &b ) || a.empty() ) return false;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) < b( i1, i2, i3 ) ) ) return false;
				}
			}
		}
		return true;
	}

	// Slice <= Slice
	inline
	friend
	bool
	le( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( ( &a == &b ) || a.empty() ) return true;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) <= b( i1, i2, i3 ) ) ) return false;
				}
			}
		}
		return true;
	}

	// Slice > Slice
	inline
	friend
	bool
	gt( FArray3S const & a, FArray3S const & b )
	{
		return lt( b, a );
	}

	// Slice >= Slice
	inline
	friend
	bool
	ge( FArray3S const & a, FArray3S const & b )
	{
		return le( b, a );
	}

	// Slice == Value
	inline
	friend
	bool
	eq( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return true;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) == t ) ) return false;
				}
			}
		}
		return true;
	}

	// Slice != Value
	inline
	friend
	bool
	ne( FArray3S const & a, T const & t )
	{
		return ! eq( a, t );
	}

	// Slice < Value
	inline
	friend
	bool
	lt( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) < t ) ) return false;
				}
			}
		}
		return true;
	}

	// Slice <= Value
	inline
	friend
	bool
	le( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return true;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) <= t ) ) return false;
				}
			}
		}
		return true;
	}

	// Slice > Value
	inline
	friend
	bool
	gt( FArray3S const & a, T const & t )
	{
		return lt( t, a );
	}

	// Slice >= Value
	inline
	friend
	bool
	ge( FArray3S const & a, T const & t )
	{
		return le( t, a );
	}

	// Value == Slice
	inline
	friend
	bool
	eq( T const & t, FArray3S const & a )
	{
		return eq( a, t );
	}

	// Value != Slice
	inline
	friend
	bool
	ne( T const & t, FArray3S const & a )
	{
		return ! eq( a, t );
	}

	// Value < Slice
	inline
	friend
	bool
	lt( T const & t, FArray3S const & a )
	{
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( t < a( i1, i2, i3 ) ) ) return false;
				}
			}
		}
		return true;
	}

	// Value <= Slice
	inline
	friend
	bool
	le( T const & t, FArray3S const & a )
	{
		if ( a.empty() ) return true;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( t <= a( i1, i2, i3 ) ) ) return false;
				}
			}
		}
		return true;
	}

	// Value > Slice
	inline
	friend
	bool
	gt( T const & t, FArray3S const & a )
	{
		return lt( a, t );
	}

	// Value >= Slice
	inline
	friend
	bool
	ge( T const & t, FArray3S const & a )
	{
		return le( a, t );
	}

public: // Comparison: Predicate: Any

	// Any Slice == Slice
	inline
	friend
	bool
	any_eq( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return false;
		if ( &a == &b ) return true;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) == b( i1, i2, i3 ) ) return true;
				}
			}
		}
		return false;
	}

	// Any Slice != Slice
	inline
	friend
	bool
	any_ne( FArray3S const & a, FArray3S const & b )
	{
		return ! eq( a, b );
	}

	// Any Slice < Slice
	inline
	friend
	bool
	any_lt( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return false;
		if ( &a == &b ) return false;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) < b( i1, i2, i3 ) ) return true;
				}
			}
		}
		return false;
	}

	// Any Slice <= Slice
	inline
	friend
	bool
	any_le( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return false;
		if ( &a == &b ) return true;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) <= b( i1, i2, i3 ) ) return true;
				}
			}
		}
		return false;
	}

	// Any Slice > Slice
	inline
	friend
	bool
	any_gt( FArray3S const & a, FArray3S const & b )
	{
		return any_lt( b, a );
	}

	// Any Slice >= Slice
	inline
	friend
	bool
	any_ge( FArray3S const & a, FArray3S const & b )
	{
		return any_le( b, a );
	}

	// Any Slice == Value
	inline
	friend
	bool
	any_eq( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) == t ) return true;
				}
			}
		}
		return false;
	}

	// Any Slice != Value
	inline
	friend
	bool
	any_ne( FArray3S const & a, T const & t )
	{
		return ! eq( a, t );
	}

	// Any Slice < Value
	inline
	friend
	bool
	any_lt( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) < t ) return true;
				}
			}
		}
		return false;
	}

	// Any Slice <= Value
	inline
	friend
	bool
	any_le( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) <= t ) return true;
				}
			}
		}
		return false;
	}

	// Any Slice > Value
	inline
	friend
	bool
	any_gt( FArray3S const & a, T const & t )
	{
		return any_lt( t, a );
	}

	// Any Slice >= Value
	inline
	friend
	bool
	any_ge( FArray3S const & a, T const & t )
	{
		return any_le( t, a );
	}

	// Any Value == Slice
	inline
	friend
	bool
	any_eq( T const & t, FArray3S const & a )
	{
		return any_eq( a, t );
	}

	// Any Value != Slice
	inline
	friend
	bool
	any_ne( T const & t, FArray3S const & a )
	{
		return ! eq( a, t );
	}

	// Any Value < Slice
	inline
	friend
	bool
	any_lt( T const & t, FArray3S const & a )
	{
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( t < a( i1, i2, i3 ) ) return true;
				}
			}
		}
		return false;
	}

	// Any Value <= Slice
	inline
	friend
	bool
	any_le( T const & t, FArray3S const & a )
	{
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( t <= a( i1, i2, i3 ) ) return true;
				}
			}
		}
		return false;
	}

	// Any Value > Slice
	inline
	friend
	bool
	any_gt( T const & t, FArray3S const & a )
	{
		return any_lt( a, t );
	}

	// Any Value >= Slice
	inline
	friend
	bool
	any_ge( T const & t, FArray3S const & a )
	{
		return any_le( a, t );
	}

public: // Comparison: Predicate: All

	// All Slice == Slice
	inline
	friend
	bool
	all_eq( FArray3S const & a, FArray3S const & b )
	{
		return eq( a, b );
	}

	// All Slice != Slice
	inline
	friend
	bool
	all_ne( FArray3S const & a, FArray3S const & b )
	{
		return ! any_eq( a, b );
	}

	// All Slice < Slice
	inline
	friend
	bool
	all_lt( FArray3S const & a, FArray3S const & b )
	{
		return lt( a, b );
	}

	// All Slice <= Slice
	inline
	friend
	bool
	all_le( FArray3S const & a, FArray3S const & b )
	{
		return le( a, b );
	}

	// All Slice > Slice
	inline
	friend
	bool
	all_gt( FArray3S const & a, FArray3S const & b )
	{
		return gt( a, b );
	}

	// All Slice >= Slice
	inline
	friend
	bool
	all_ge( FArray3S const & a, FArray3S const & b )
	{
		return ge( a, b );
	}

	// All Slice == Value
	inline
	friend
	bool
	all_eq( FArray3S const & a, T const & t )
	{
		return eq( a, t );
	}

	// All Slice != Value
	inline
	friend
	bool
	all_ne( FArray3S const & a, T const & t )
	{
		return ! any_eq( a, t );
	}

	// All Slice < Value
	inline
	friend
	bool
	all_lt( FArray3S const & a, T const & t )
	{
		return lt( a, t );
	}

	// All Slice <= Value
	inline
	friend
	bool
	all_le( FArray3S const & a, T const & t )
	{
		return le( a, t );
	}

	// All Slice > Value
	inline
	friend
	bool
	all_gt( FArray3S const & a, T const & t )
	{
		return gt( a, t );
	}

	// All Slice >= Value
	inline
	friend
	bool
	all_ge( FArray3S const & a, T const & t )
	{
		return ge( a, t );
	}

	// All Value == Slice
	inline
	friend
	bool
	all_eq( T const & t, FArray3S const & a )
	{
		return eq( t, a );
	}

	// All Value != Slice
	inline
	friend
	bool
	all_ne( T const & t, FArray3S const & a )
	{
		return ! any_eq( t, a );
	}

	// All Value < Slice
	inline
	friend
	bool
	all_lt( T const & t, FArray3S const & a )
	{
		return lt( t, a );
	}

	// All Value <= Slice
	inline
	friend
	bool
	all_le( T const & t, FArray3S const & a )
	{
		return le( t, a );
	}

	// All Value > Slice
	inline
	friend
	bool
	all_gt( T const & t, FArray3S const & a )
	{
		return gt( t, a );
	}

	// All Value >= Slice
	inline
	friend
	bool
	all_ge( T const & t, FArray3S const & a )
	{
		return ge( t, a );
	}

public: // Comparison: Count

	// Count Slice == Slice
	inline
	friend
	size_type
	count_eq( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		if ( &a == &b ) return a.size_;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) == b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count Slice != Slice
	inline
	friend
	size_type
	count_ne( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		if ( &a == &b ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) != b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count Slice < Slice
	inline
	friend
	size_type
	count_lt( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		if ( &a == &b ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) < b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count Slice <= Slice
	inline
	friend
	size_type
	count_le( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		if ( &a == &b ) return a.size_;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) <= b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count Slice > Slice
	inline
	friend
	size_type
	count_gt( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		if ( &a == &b ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) > b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count Slice >= Slice
	inline
	friend
	size_type
	count_ge( FArray3S const & a, FArray3S const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		if ( &a == &b ) return a.size_;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) >= b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count Slice == Value
	inline
	friend
	size_type
	count_eq( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) == t ) ++n;
				}
			}
		}
		return n;
	}

	// Count Value == Slice
	inline
	friend
	size_type
	count_eq( T const & t, FArray3S const & a )
	{
		return count_eq( a, t );
	}

	// Count Slice != Value
	inline
	friend
	size_type
	count_ne( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) != t ) ++n;
				}
			}
		}
		return n;
	}

	// Count Value != Slice
	inline
	friend
	size_type
	count_ne( T const & t, FArray3S const & a )
	{
		return count_ne( a, t );
	}

	// Count Slice < Value
	inline
	friend
	size_type
	count_lt( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) < t ) ++n;
				}
			}
		}
		return n;
	}

	// Count Value < Slice
	inline
	friend
	size_type
	count_lt( T const & t, FArray3S const & a )
	{
		return count_gt( a, t );
	}

	// Count Slice <= Value
	inline
	friend
	size_type
	count_le( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) <= t ) ++n;
				}
			}
		}
		return n;
	}

	// Count Value <= Slice
	inline
	friend
	size_type
	count_le( T const & t, FArray3S const & a )
	{
		return count_ge( a, t );
	}

	// Count Slice > Value
	inline
	friend
	size_type
	count_gt( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) > t ) ++n;
				}
			}
		}
		return n;
	}

	// Count Value > Slice
	inline
	friend
	size_type
	count_gt( T const & t, FArray3S const & a )
	{
		return count_lt( a, t );
	}

	// Count Slice >= Value
	inline
	friend
	size_type
	count_ge( FArray3S const & a, T const & t )
	{
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = a.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = a.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = a.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) >= t ) ++n;
				}
			}
		}
		return n;
	}

	// Count Value >= Slice
	inline
	friend
	size_type
	count_ge( T const & t, FArray3S const & a )
	{
		return count_le( a, t );
	}

public: // Comparison: Predicate: MArray

	// FArray3S == MArray3
	template< class A >
	inline
	friend
	bool
	eq( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return true;
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) == b( i1, i2, i3 ) ) ) return false;
				}
			}
		}
		return true;
	}

	// FArray3S != MArray3
	template< class A >
	inline
	friend
	bool
	ne( FArray3S const & a, MArray3< A, T > const & b )
	{
		return ! eq( a, b );
	}

	// FArray3S < MArray3
	template< class A >
	inline
	friend
	bool
	lt( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) < b( i1, i2, i3 ) ) ) return false;
				}
			}
		}
		return true;
	}

	// FArray3S <= MArray3
	template< class A >
	inline
	friend
	bool
	le( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return true;
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) <= b( i1, i2, i3 ) ) ) return false;
				}
			}
		}
		return true;
	}

	// FArray3S > MArray3
	template< class A >
	inline
	friend
	bool
	gt( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) > b( i1, i2, i3 ) ) ) return false;
				}
			}
		}
		return true;
	}

	// FArray3S >= MArray3
	template< class A >
	inline
	friend
	bool
	ge( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return true;
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( ! ( a( i1, i2, i3 ) >= b( i1, i2, i3 ) ) ) return false;
				}
			}
		}
		return true;
	}

	// MArray3 == FArray3S
	template< class A >
	inline
	friend
	bool
	eq( MArray3< A, T > const & a, FArray3S const & b )
	{
		return eq( b, a );
	}

	// MArray3 != FArray3S
	template< class A >
	inline
	friend
	bool
	ne( MArray3< A, T > const & a, FArray3S const & b )
	{
		return ne( b, a );
	}

	// MArray3 < FArray3S
	template< class A >
	inline
	friend
	bool
	lt( MArray3< A, T > const & a, FArray3S const & b )
	{
		return gt( b, a );
	}

	// MArray3 <= FArray3S
	template< class A >
	inline
	friend
	bool
	le( MArray3< A, T > const & a, FArray3S const & b )
	{
		return ge( b, a );
	}

	// MArray3 > FArray3S
	template< class A >
	inline
	friend
	bool
	gt( MArray3< A, T > const & a, FArray3S const & b )
	{
		return lt( b, a );
	}

	// MArray3 >= FArray3S
	template< class A >
	inline
	friend
	bool
	ge( MArray3< A, T > const & a, FArray3S const & b )
	{
		return le( b, a );
	}

public: // Comparison: Predicate: Any: MArray

	// Any FArray3S == MArray3
	template< class A >
	inline
	friend
	bool
	any_eq( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) == b( i1, i2, i3 ) ) return true;
				}
			}
		}
		return false;
	}

	// Any FArray3S != MArray3
	template< class A >
	inline
	friend
	bool
	any_ne( FArray3S const & a, MArray3< A, T > const & b )
	{
		return ! eq( a, b );
	}

	// Any FArray3S < MArray3
	template< class A >
	inline
	friend
	bool
	any_lt( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) < b( i1, i2, i3 ) ) return true;
				}
			}
		}
		return false;
	}

	// Any FArray3S <= MArray3
	template< class A >
	inline
	friend
	bool
	any_le( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) <= b( i1, i2, i3 ) ) return true;
				}
			}
		}
		return false;
	}

	// Any FArray3S > MArray3
	template< class A >
	inline
	friend
	bool
	any_gt( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) > b( i1, i2, i3 ) ) return true;
				}
			}
		}
		return false;
	}

	// Any FArray3S >= MArray3
	template< class A >
	inline
	friend
	bool
	any_ge( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return false;
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) >= b( i1, i2, i3 ) ) return true;
				}
			}
		}
		return false;
	}

	// Any MArray3 == FArray3S
	template< class A >
	inline
	friend
	bool
	any_eq( MArray3< A, T > const & a, FArray3S const & b )
	{
		return any_eq( b, a );
	}

	// Any MArray3 != FArray3S
	template< class A >
	inline
	friend
	bool
	any_ne( MArray3< A, T > const & a, FArray3S const & b )
	{
		return any_ne( b, a );
	}

	// Any MArray3 < FArray3S
	template< class A >
	inline
	friend
	bool
	any_lt( MArray3< A, T > const & a, FArray3S const & b )
	{
		return any_gt( b, a );
	}

	// Any MArray3 <= FArray3S
	template< class A >
	inline
	friend
	bool
	any_le( MArray3< A, T > const & a, FArray3S const & b )
	{
		return any_ge( b, a );
	}

	// Any MArray3 > FArray3S
	template< class A >
	inline
	friend
	bool
	any_gt( MArray3< A, T > const & a, FArray3S const & b )
	{
		return any_lt( b, a );
	}

	// Any MArray3 >= FArray3S
	template< class A >
	inline
	friend
	bool
	any_ge( MArray3< A, T > const & a, FArray3S const & b )
	{
		return any_le( b, a );
	}

public: // Comparison: Predicate: All: MArray

	// All FArray3S == MArray3
	template< class A >
	inline
	friend
	bool
	all_eq( FArray3S const & a, MArray3< A, T > const & b )
	{
		return eq( a, b );
	}

	// All FArray3S != MArray3
	template< class A >
	inline
	friend
	bool
	all_ne( FArray3S const & a, MArray3< A, T > const & b )
	{
		return ! any_eq( a, b );
	}

	// All FArray3S < MArray3
	template< class A >
	inline
	friend
	bool
	all_lt( FArray3S const & a, MArray3< A, T > const & b )
	{
		return lt( a, b );
	}

	// All FArray3S <= MArray3
	template< class A >
	inline
	friend
	bool
	all_le( FArray3S const & a, MArray3< A, T > const & b )
	{
		return le( a, b );
	}

	// All FArray3S > MArray3
	template< class A >
	inline
	friend
	bool
	all_gt( FArray3S const & a, MArray3< A, T > const & b )
	{
		return gt( a, b );
	}

	// All FArray3S >= MArray3
	template< class A >
	inline
	friend
	bool
	all_ge( FArray3S const & a, MArray3< A, T > const & b )
	{
		return ge( a, b );
	}

	// All MArray3 == FArray3S
	template< class A >
	inline
	friend
	bool
	all_eq( MArray3< A, T > const & a, FArray3S const & b )
	{
		return all_eq( b, a );
	}

	// All MArray3 != FArray3S
	template< class A >
	inline
	friend
	bool
	all_ne( MArray3< A, T > const & a, FArray3S const & b )
	{
		return all_ne( b, a );
	}

	// All MArray3 < FArray3S
	template< class A >
	inline
	friend
	bool
	all_lt( MArray3< A, T > const & a, FArray3S const & b )
	{
		return all_gt( b, a );
	}

	// All MArray3 <= FArray3S
	template< class A >
	inline
	friend
	bool
	all_le( MArray3< A, T > const & a, FArray3S const & b )
	{
		return all_ge( b, a );
	}

	// All MArray3 > FArray3S
	template< class A >
	inline
	friend
	bool
	all_gt( MArray3< A, T > const & a, FArray3S const & b )
	{
		return all_lt( b, a );
	}

	// All MArray3 >= FArray3S
	template< class A >
	inline
	friend
	bool
	all_ge( MArray3< A, T > const & a, FArray3S const & b )
	{
		return all_le( b, a );
	}

public: // Comparison: Count: MArray

	// Count FArray3S == MArray3
	template< class A >
	inline
	friend
	size_type
	count_eq( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) == b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count FArray3S != MArray3
	template< class A >
	inline
	friend
	size_type
	count_ne( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) != b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count FArray3S < MArray3
	template< class A >
	inline
	friend
	size_type
	count_lt( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) < b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count FArray3S <= MArray3
	template< class A >
	inline
	friend
	size_type
	count_le( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) <= b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count FArray3S > MArray3
	template< class A >
	inline
	friend
	size_type
	count_gt( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) > b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count FArray3S >= MArray3
	template< class A >
	inline
	friend
	size_type
	count_ge( FArray3S const & a, MArray3< A, T > const & b )
	{
		assert( a.conformable( b ) );
		if ( a.empty() ) return 0;
		size_type n( 0 );
		for ( int i3 = 1, e3 = b.u3(); i3 <= e3; ++i3 ) {
			for ( int i2 = 1, e2 = b.u2(); i2 <= e2; ++i2 ) {
				for ( int i1 = 1, e1 = b.u1(); i1 <= e1; ++i1 ) {
					if ( a( i1, i2, i3 ) >= b( i1, i2, i3 ) ) ++n;
				}
			}
		}
		return n;
	}

	// Count MArray3 == FArray3S
	template< class A >
	inline
	friend
	size_type
	count_eq( MArray3< A, T > const & a, FArray3S const & b )
	{
		return count_eq( b, a );
	}

	// Count MArray3 != FArray3S
	template< class A >
	inline
	friend
	size_type
	count_ne( MArray3< A, T > const & a, FArray3S const & b )
	{
		return count_ne( b, a );
	}

	// Count MArray3 < FArray3S
	template< class A >
	inline
	friend
	size_type
	count_lt( MArray3< A, T > const & a, FArray3S const & b )
	{
		return count_gt( b, a );
	}

	// Count MArray3 <= FArray3S
	template< class A >
	inline
	friend
	size_type
	count_le( MArray3< A, T > const & a, FArray3S const & b )
	{
		return count_ge( b, a );
	}

	// Count MArray3 > FArray3S
	template< class A >
	inline
	friend
	size_type
	count_gt( MArray3< A, T > const & a, FArray3S const & b )
	{
		return count_lt( b, a );
	}

	// Count MArray3 >= FArray3S
	template< class A >
	inline
	friend
	size_type
	count_ge( MArray3< A, T > const & a, FArray3S const & b )
	{
		return count_le( b, a );
	}

private: // Methods

	// Memory Range Set
	inline
	void
	data_set()
	{
		if ( size_ > 0u ) { // Non-empty slice
			data_beg_ = data_end_ = data_ + k_;
			data_beg_ += m1_ * ( m1_ >= 0 ? 1 : u1_ );
			data_end_ += m1_ * ( m1_ <= 0 ? 1 : u1_ );
			data_beg_ += m2_ * ( m2_ >= 0 ? 1 : u2_ );
			data_end_ += m2_ * ( m2_ <= 0 ? 1 : u2_ );
			data_beg_ += m3_ * ( m3_ >= 0 ? 1 : u3_ );
			data_end_ += m3_ * ( m3_ <= 0 ? 1 : u3_ );
		} else {
			data_ = data_beg_ = data_end_ = nullptr;
		}
	}

private: // Data

	std::int64_t m1_; // Multiplier of dim 1
	std::int64_t m2_; // Multiplier of dim 2
	std::int64_t m3_; // Multiplier of dim 3
	std::int64_t k_; // Constant
	int u1_; // Upper index of dim 1
	int u2_; // Upper index of dim 2
	int u3_; // Upper index of dim 3

}; // FArray3S

// Conformable?
template< typename U, typename V >
inline
bool
conformable( FArray3S< U > const & a, FArray3S< V > const & b )
{
	return a.conformable( b );
}

// Conformable?
template< typename U, class A, typename M >
inline
bool
conformable( FArray3S< U > const & a, MArray3< A, M > const & b )
{
	return a.conformable( b );
}

// Conformable?
template< class A, typename M, typename V >
inline
bool
conformable( MArray3< A, M > const & a, FArray3S< V > const & b )
{
	return b.conformable( a );
}

} // ObjexxFCL

#endif // ObjexxFCL_FArray3S_hh_INCLUDED
