/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import { bindActionCreators } from 'redux'

import defaultTheme from 'brave-ui/theme/brave-default'
import { ThemeProvider } from 'brave-ui/theme'
require('../fonts/muli.css')
require('../fonts/poppins.css')

// Components
import App from './screens/app'

// Utils
import store from './store'
import * as webcompatReporterActions from './actions/webcompatreporter_actions'

let actions: any

window.cr.define('brave_webcompat_reporter', function () {
  'use strict'

  function loadDialogArgs () {
    const dialogArgsRaw = chrome.getVariableValue('dialogArguments')
    let dialogArgs
    try {
      dialogArgs = JSON.parse(dialogArgsRaw)
    } catch (e) {
      console.error('Error parsing incoming dialog args: ', dialogArgsRaw, e)
    }

    getActions().setSiteUrl(dialogArgs.siteUrl)
  }

  function initialize () {
    loadDialogArgs()

    render(
      <Provider store={store}>
        <ThemeProvider theme={defaultTheme}>
          <App />
        </ThemeProvider>
      </Provider>,
      document.getElementById('root'))
  }

  function getActions () {
    if (actions) {
      return actions
    }

    actions = bindActionCreators(webcompatReporterActions, store.dispatch.bind(store))
    return actions
  }

  return {
    initialize
  }
})

document.addEventListener('DOMContentLoaded', window.brave_webcompat_reporter.initialize)
