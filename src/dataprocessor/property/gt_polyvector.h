/* GTlab - Gas Turbine laboratory
 * Source File: gt_structproperty.h
 * copyright 2022 by DLR
 *
 *  Author: Martin Siggel (AT-TW)
 *  Tel.: +49 2203 601 2264
 */

#ifndef GT_POLYVECTOR_H
#define GT_POLYVECTOR_H

#include <vector>
#include <memory>

namespace gt
{

    /**
 * @brief The poly_vector class implements a vector of polymorphic objects.
 *
 * It replaces something like std::vector<BaseClass*> with
 * gt::poly_vector<BaseClass>. It removes the memory management from
 * the user but keeps the same semantics as std::vector.
 *
 * Note: As pushing elements into the container moves the ownership to
 * the container, a unique_ptr has to be passed or a copy of the object
 * must be creatible otherwise.
 */
    template <typename BaseType>
    class poly_vector
    {
    public:
        struct iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = BaseType;
            using pointer = BaseType*; // or also value_type*
            using reference = BaseType&;

            friend poly_vector;

            BaseType& operator*()
            {
                return **current;
            }

            bool operator==(const iterator& other) const
            {
                return current == other.current;
            }


            bool operator!=(const iterator& other) const
            {
                return !operator==(other);
            }

            iterator& operator++()
            {
                ++current;

                return *this;
            }


        private:
            explicit iterator(
                typename std::vector<std::unique_ptr<BaseType>>::iterator it) :
                current(it)
            {
            }

            typename std::vector<std::unique_ptr<BaseType>>::iterator current;
        };

        struct const_iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = BaseType;
            using pointer = BaseType*; // or also value_type*
            using reference = BaseType&;

            friend poly_vector;

            const BaseType& operator*()
            {
                return **current;
            }

            bool operator==(const const_iterator& other) const
            {
                return current == other.current;
            }

            bool operator!=(const const_iterator& other) const
            {
                return !operator==(other);
            }

            const_iterator& operator++()
            {
                ++current;

                return *this;
            }


        private:
            explicit const_iterator(
                typename std::vector<std::unique_ptr<BaseType>>::const_iterator
                    it) :
                current(it)
            {
            }

            typename std::vector<std::unique_ptr<BaseType>>::const_iterator
                current;
        };

        poly_vector() = default;


        /**
         * @brief Pushes back the value into the container.
         *
         * The ownership will be transfered into the container.
         * Derived does not need to be copyable
         *
         * @param v_ptr UniquePtr to a Derived value
         */
        template <typename Derived,
                  std::enable_if_t<std::is_base_of<BaseType, Derived>::value,
                                   bool> = true>
        void push_back(std::unique_ptr<Derived> v_ptr)
        {
            values.emplace_back(std::move(v_ptr));
        }

        /**
         * @brief Normal push_back functionality, Derived must be copyable!
         *
         * @param v Value to push
         */
        template <typename Derived,
                  std::enable_if_t<std::is_base_of<BaseType, Derived>::value,
                                   bool> = true>
        void push_back(Derived v)
        {
            // create a copy using Derived's move constructor
            auto ptr = std::make_unique<Derived>(std::move(v));
            values.emplace_back(std::move(ptr));
        }
        /**
     * @brief In place creation at the end of the vector with
     * the argument of the constructor of derived
     *
     * Example: Assuming a constructor of Derived(string, int),
     *
     * gt::poly_vector<Base> v;
     * v.emplace_back<Derived>("astring", 1):
     *
     * @param params
     */
        template <typename Derived, typename... Args,
                  std::enable_if_t<std::is_base_of<BaseType, Derived>::value,
                                   bool> = true>
        void emplace_back(Args&&... args)
        {
            auto ptr = std::make_unique<Derived>(std::forward<Args>(args)...);
            values.emplace_back(std::move(ptr));
        }

        BaseType& operator[](size_t index)
        {
            return *values[index];
        }

        const BaseType& operator[](size_t index) const
        {
            return *values[index];
        }

        size_t size() const
        {
            return values.size();
        }

        iterator begin()
        {
            return iterator(values.begin());
        }

        const_iterator begin() const
        {
            return const_iterator(values.begin());
        }

        iterator end()
        {
            return iterator(values.end());
        }

        const_iterator end() const
        {
            return const_iterator(values.end());
        }

        BaseType& at(size_t idx)
        {
            return *values.at(idx);
        }

        const BaseType& at(size_t idx) const
        {
            return *values.at(idx);
        }

        void clear()
        {
            values.clear();
        }

    private:
        std::vector<std::unique_ptr<BaseType>> values;
    };

} // namespace gt


#endif // GT_POLYVECTOR_H
