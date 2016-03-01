;;; geoip.el --- libGeoIP binding of Emacs Lisp

;; Copyright (C) 2016 by Syohei YOSHIDA

;; Author: Syohei YOSHIDA <syohex@gmail.com>
;; URL: https://github.com/syohex/
;; Version: 0.01

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.

;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

;;; Commentary:

;;; Code:

(require 'geoip-core)
(require 'cl-lib)
(require 'subr-x)

;;;###autoload
(defun geoip-init (db)
  (cl-assert (and (stringp db) (file-exists-p db)))
  (geoip-core-init db))

(defun geoip-record-by-name (geoip host)
  (cl-assert (stringp host))
  (geoip-core-record-by-name geoip host))

(defun geoip-record-by-addr (geoip addr)
  (cl-assert (stringp addr))
  (geoip-core-record-by-addr geoip addr))

(defun geoip-country-code (record)
  (cl-assert (null (not record)))
  (geoip-core-country-code record))

(defun geoip-region (record)
  (cl-assert (null (not record)))
  (geoip-core-region record))

(defun geoip-region-name (record)
  (cl-assert (null (not record)))
  (geoip-core-region-name record))

(defun geoip-city (record)
  (cl-assert (null (not record)))
  (geoip-core-city record))

(defun geoip-postal-code (record)
  (cl-assert (null (not record)))
  (geoip-core-postal-code record))

(defun geoip-latitude (record)
  (cl-assert (null (not record)))
  (geoip-core-latitude record))

(defun geoip-longitude (record)
  (cl-assert (null (not record)))
  (geoip-core-longitude record))

(defun geoip-metro-code (record)
  (cl-assert (null (not record)))
  (geoip-core-metro-code record))

(defun geoip-area-code (record)
  (cl-assert (null (not record)))
  (geoip-core-area-code record))

(defun geoip-time-zone (record)
  (cl-assert (null (not record)))
  (geoip-core-time-zone record))

(provide 'geoip)

;;; geoip.el ends here
