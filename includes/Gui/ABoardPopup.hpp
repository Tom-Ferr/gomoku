#ifndef ABOARDPOPUP_HPP
# define ABOARDPOPUP_HPP

# include <Rect.hpp>
# include <MLX42/MLX42.h>
# include <common.hpp>
# include <Text.hpp>

/*
** Abstract class for popups, which are used to display messages
** on the board.
** It consists mainly of a background, a center box, a header
** and an ok button, and does most of the work to display and
** resize them, making the derived classes only need to implement
** the resize of its own elements.
*/
class ABoardPopup
{
	protected:
		mlx_image_t						*_background;
		mlx_image_t						*_centerbox;
		mlx_image_t						*_header;
		mlx_image_t						*_ok_button;
		mlx_image_t						*_ok_button_hover;
		Rect							_dimensions;
		Rect							_ok_dimensions;
		Text							_header_text;
		bool							_enabled;

		void _resize_background();
		void _resize_box(bool wide=false);
		void _box_texture();
		void _resize_ok();
		void _resize_header();
        bool _init();
        void _resize();
        void _show();
        void _hide();
        void _hover();
        bool _click();
        void _set_header_text(std::string const &text);

	public:
		ABoardPopup();
		ABoardPopup(ABoardPopup const &other);
		ABoardPopup &operator=(ABoardPopup const &other);
		Rect const &dimensions();
        bool enabled();
		virtual ~ABoardPopup();
		virtual bool init() = 0;
		virtual void resize() = 0;
        virtual bool click() = 0;
		virtual void hide() = 0;
		virtual void hover() = 0;
};

#endif
